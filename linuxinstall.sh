#!/bin/bash
cp -r * ../
 
cd ../
cp -r bin/x64/* .
DIR="$(pwd)"
echo $DIR
steam() {
    echo $DIR
    b=${DIR%/steamapps*}
    b=$b/steamapps/compatdata/1091500/pfx/
    echo " "
    echo " "
    
    echo "I am going to run this command, are you sure?"
    echo "WINEPREFIX=$b /bin/wine64 regedit EnableSignatureOverride.reg"
    read -p "Press [Enter] to continue..."
    WINEPREFIX=$b /bin/wine64 regedit EnableSignatureOverride.reg
}

heroic(){
    home=$(echo ~)
    home=$home/Games/Heroic/Prefixes/cyberpunk-2077
    echo "I am going to run this command, are you sure?"
    echo "WINEPREFIX=$home /bin/wine64 regedit EnableSignatureOverride.reg"
    read -p "Press [Enter] to continue..."
    WINEPREFIX=$home /bin/wine64 regedit EnableSignatureOverride.reg

}

gog(){
    home=$(echo ~)
    home=$home/Games/cyberpunk-2077
    echo "I am going to run this command, are you sure?"
    echo "WINEPREFIX=$home /bin/wine64 regedit EnableSignatureOverride.reg"
    read -p "Press [Enter] to continue..."
    WINEPREFIX=$home /bin/wine64 regedit EnableSignatureOverride.reg

}
if [ -f "$DIR/Cyberpunk2077.exe" ]; then
    echo "what launcher are you using?"
    echo "1. Steam"
    echo "2. Gog (lutris)"
    echo "3. Heroic (epic games, it probably also works for gog tho) "
    read -p "Enter your choice: " choice
    case $choice in
        1)
            steam
            ;;
        2)
            gog
            ;;
        3)
            heroic
            ;;
        *)
            echo "Invalid choice"
            ;;
    esac
else
    echo "Cyberpunk2077.exe not found, please extract the files in the directory where Cyberpunk2077.exe is stored and try again"
    exit 1
fi 

