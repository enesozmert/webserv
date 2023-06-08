<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $targetDir = './Website_to_test/uploads/'; // Dosyanın kaydedileceği dizin
    $fileName = $_FILES['file']['name'];
    $targetFile = $targetDir . $fileName; // Hedef dosyanın tam yolu
    
    // Dosyayı belirtilen dizine kaydet
    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFile)) {
        echo "Dosya basarıyla yuklendi: " . $fileName;
    } else {
        echo "Dosya yukleme hatasi.";
    }
}
?>