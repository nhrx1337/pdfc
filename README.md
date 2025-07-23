# PDFC


## 📋 Overview
This project is a simple PDF viewer built using **C**, **GTK3**, and **Poppler**. It allows users to open and view PDF files with basic navigation features. The application is designed to be lightweight.

You can install dependencies on your system using the following commands:
### Debian-based Systems
```bash
sudo apt-get install libgtk-3-dev libpoppler-glib-dev
```
### Arch-based Systems
```bash
sudo pacman -S gtk3 poppler
```
## 📥 Installation
1. **Clone the repository:**
```bash
git clone https://github.com/nhrx1337/pdfc.git
cd pdfc/
```
2. **Compile the application:**
```bash
make
```
3. **Run the application:**
```bash
./pdfc <path/to/your/file.pdf>
```
**or open directly:**
```bash
./pdfc
```

