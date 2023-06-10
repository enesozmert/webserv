<?php
  if(isset($_POST['say'])){
    // $say = htmlspecialchars($_POST['say']);
    // $to = htmlspecialchars($_POST['to']);
    $say = $_ENV['say'];
    $to = $_ENV['to'];

    echo '>>>>' . $say . " " . $to;
  }
  ?>