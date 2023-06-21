<?php
if ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
    $data = json_decode(file_get_contents('php://input'), true);
    $filePath = $data['filePath'];

    // DELETE işlemini burada gerçekleştirin
    // Örneğin, bir dosyayı silmek için unlink() fonksiyonunu kullanabilirsiniz
    if (unlink($filePath)) {
        http_response_code(200);
        echo 'File deleted successfully.';
    } else {
        http_response_code(500);
        echo 'Error deleting the file.';
    }
} else {
    http_response_code(405); // Method Not Allowed
    echo 'Invalid request method.';
}
?>
