<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $targetDir = './uploads/'; // Dosyanın kaydedileceği dizin
    $fileName = $_FILES['file']['name'];
    $targetFile = $targetDir . $fileName; // Hedef dosyanın tam yolu
    echo ">>>".basename($_FILES["file"]["name"])."</br>";
    // Dosyayı belirtilen dizine kaydet
    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFile)) {
        echo "Dosya başarıyla yüklendi: " . $fileName;
    } else {
        echo "Dosya yükleme hatası.";
    }
}
?>