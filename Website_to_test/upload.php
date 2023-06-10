<!DOCTYPE html>
<html>
<head>
  <title>Veri Gönderme Formu</title>
</head>
<body>
  <form action="#" method="post">
    <label for="say">Sayı:</label>
    <input type="text" name="say" id="say"><br>

    <label for="to">Hedef:</label>
    <input type="text" name="to" id="to"><br>

    <input type="submit" value="Gönder">
  </form>

  <?php
  if(isset($_POST['say'])){
    $say = htmlspecialchars($_POST['say']);
    $to = htmlspecialchars($_POST['to']);
    // $say = $_ENV['say'];
    // $to = $_ENV['to'];

    echo '>>>>' . $say . " " . $to;
  }
  ?>

</body>
</html>

