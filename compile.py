import sys
import os
import subprocess
import shutil
from pathlib import Path
from PySide6.QtWidgets import (QApplication, QMainWindow, QPushButton, QVBoxLayout, 
                             QHBoxLayout, QWidget, QTextEdit, QLabel, QCheckBox)
import platform

class SDL3Builder(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("SDL3 Multi-Directory Builder")
        self.resize(900, 600)

        self.root_dir = Path(os.getcwd())
        self.obj_dir = self.root_dir / "obj"
        self.sdl_dir = self.root_dir / "SDL3"
        self.bin_name = "app_sdl3.exe" if os.name == 'nt' else "app_sdl3"
        
        if not self.obj_dir.exists():
            self.obj_dir.mkdir(parents=True)

        self.init_ui()

    def init_ui(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)

        self.status_label = QLabel(f"📂 Racine : {self.root_dir}")
        layout.addWidget(self.status_label)

        self.console = QTextEdit()
        self.console.setReadOnly(True)
        self.console.setStyleSheet("background-color: #1e1e1e; color: #71f08e; font-family: 'Consolas'; font-size: 12px;")
        layout.addWidget(self.console)

        btn_layout = QHBoxLayout()
        self.btn_compile = QPushButton("🔨 Build Projet (Récursif)")
        self.btn_compile.clicked.connect(self.run_full_build)
        
        self.btn_run = QPushButton("🚀 Lancer")
        self.btn_run.clicked.connect(self.run_executable)
        
        self.btn_clean = QPushButton("🧹 Nettoyer (supprimer obj + binaire)")
        self.btn_clean.clicked.connect(self.clean_build)

        # Options pour lier les modules SDL supplémentaires
        self.chk_image = QCheckBox("SDL_image")
        self.chk_ttf = QCheckBox("SDL_ttf")
        self.chk_mixer = QCheckBox("SDL_mixer")
        # Par défaut activer
        self.chk_image.setChecked(True)
        self.chk_ttf.setChecked(True)
        self.chk_mixer.setChecked(True)

        # boutons principaux
        btn_layout.addWidget(self.btn_compile)
        btn_layout.addWidget(self.btn_run)
        btn_layout.addWidget(self.btn_clean)
        # options
        btn_layout.addWidget(self.chk_image)
        btn_layout.addWidget(self.chk_ttf)
        btn_layout.addWidget(self.chk_mixer)
        layout.addLayout(btn_layout)

    def log(self, text):
        self.console.append(text)

    def run_full_build(self):
        self.console.clear()
        self.log("🔍 Recherche des fichiers sources...")
        
        # rglob("*.cpp") cherche dans TOUS les sous-dossiers
        cpp_files = list(self.root_dir.rglob("*.cpp"))
        # On ignore les fichiers qui sont dans le dossier SDL3 lui-même
        cpp_files = [f for f in cpp_files if "SDL3" not in f.parts]

        if not cpp_files:
            self.log("❌ Aucun fichier .cpp trouvé !")
            return

        obj_files = []
        compile_errors = False

        # Détection plateforme pour choisir compilateur et flags
        system = platform.system().lower()
        arch = platform.machine().lower()

        compiler = 'clang++' if system == 'darwin' else 'g++'
        cxxflags = '-std=c++17 -O2 -fPIC'

        inc_path = f'-I"{self.sdl_dir / "include"}"'
        inc_project = f'-I"{self.root_dir}"'

        # Choix du dossier lib selon architecture si présent
        lib_dir = self.sdl_dir / 'lib'
        # recherche basique: prefer platform-specific subfolder
        for candidate in (arch, 'x64', 'x86', 'arm64'):
            candidate_path = lib_dir / candidate
            if candidate_path.exists():
                lib_path = f'-L"{candidate_path}"'
                break
        else:
            lib_path = f'-L"{lib_dir}"'

        # libs de base et optionnels
        libs = ['-lSDL3']
        if self.chk_image.isChecked(): libs.append('-lSDL3_image')
        if self.chk_ttf.isChecked(): libs.append('-lSDL3_ttf')
        if self.chk_mixer.isChecked(): libs.append('-lSDL3_mixer')
        # libs système usuelles
        if system == 'linux':
            libs.extend(['-lm','-ldl','-lpthread'])
        elif system == 'darwin':
            # sur mac, garder lien simple; l'utilisateur peut ajuster si besoin
            libs.append('-lm')

        libs = ' '.join(libs)

        for cpp in cpp_files:
            # Création d'un nom d'objet unique basé sur le chemin relatif
            # Exemple: src/main.cpp -> obj/src_main.o
            relative_path = cpp.relative_to(self.root_dir)
            obj_name = str(relative_path).replace(os.sep, "_").replace(".cpp", ".o")
            obj = self.obj_dir / obj_name
            
            obj_files.append(f'"{obj}"')

            if not obj.exists() or cpp.stat().st_mtime > obj.stat().st_mtime:
                self.log(f"Compiling: {relative_path}...")
                cmd = f'{compiler} -c "{cpp}" -o "{obj}" {cxxflags} {inc_path} {inc_project}'
                res = subprocess.run(cmd, shell=True, capture_output=True, text=True)
                
                if res.returncode != 0:
                    self.log(f"❌ Erreur dans {cpp.name}:\n{res.stderr}")
                    compile_errors = True
                    break
            else:
                self.log(f"✅ {relative_path} est à jour.")

        if compile_errors: return

        self.log("\n=== Linkage final ===")
        all_objs = " ".join(obj_files)
        link_cmd = f'{compiler} {all_objs} -o "{self.bin_name}" {lib_path} {libs}'

        self.log(f"Link command: {link_cmd}")
        res = subprocess.run(link_cmd, shell=True, capture_output=True, text=True)
        if res.returncode == 0:
            self.log(f"🎉 Succès ! '{self.bin_name}' généré avec succès.")
            self.copy_sdl_dll()
        else:
            self.log(f"❌ Erreur de linkage :\n{res.stderr}")

    def copy_sdl_dll(self):
        system = platform.system().lower()
        # copier les bibliothèques partagées présentes dans SDL3/lib ou sous-dossiers
        lib_base = self.sdl_dir / 'lib'
        copied = 0
        for p in lib_base.rglob('*SDL3*'):
            if p.is_file() and p.suffix in ('.dll', '.so', '.dylib'):
                try:
                    shutil.copy2(p, self.root_dir / p.name)
                    self.log(f"📦 Copié {p.name} à la racine.")
                    copied += 1
                except Exception as e:
                    self.log(f"⚠️ Impossible de copier {p.name}: {e}")
        if copied == 0:
            self.log("ℹ️ Aucune bibliothèque SDL3 trouvée à copier (vérifiez SDL3/lib).")

    def run_executable(self):
        exe_path = self.root_dir / self.bin_name
        if exe_path.exists():
            self.log(f"Lancement de {self.bin_name}...")
            # On s'assure que le dossier de travail est la racine
            cmd = [str(exe_path)] if os.name == 'nt' else [f"./{self.bin_name}"]
            subprocess.Popen(cmd, cwd=str(self.root_dir), shell=True)
        else:
            self.log("❌ Exécutable introuvable.")

    def clean_build(self):
        # supprimer fichiers objets et binaire
        removed = 0
        if self.obj_dir.exists():
            for f in self.obj_dir.iterdir():
                try:
                    f.unlink()
                    removed += 1
                except Exception:
                    pass
        binf = self.root_dir / self.bin_name
        if binf.exists():
            try:
                binf.unlink()
                removed += 1
            except Exception:
                pass
        self.log(f"🧹 Nettoyage effectué, {removed} fichiers supprimés.")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = SDL3Builder()
    window.show()
    sys.exit(app.exec())