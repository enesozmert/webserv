<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $targetDir = '/Website_to_test/Upload/'; // Dosyanın kaydedileceği dizin
    $fileName = $_FILES['file']['name']; // $_FILES['filename'] -> $_FILES['file']
    $targetFile = $targetDir . $fileName; // Hedef dosyanın tam yolu
    echo ">>>" . basename($fileName) . "</br>"; // $_FILES['filename']['name'] -> $fileName
    
    // Dosyayı belirtilen dizine kaydet
    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFile)) { // $_FILES['filename'] -> $_FILES['file']
        echo "Dosya basarıyla yuklendi: " . $fileName;
    } else {
        echo "Dosya yukleme hatasi.";
    }
}
?>
