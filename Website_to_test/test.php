<?php

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  // Get data from fields
  $fname = $_POST['fname'];
  $lname = $_POST['lname'];

  echo '<!DOCTYPE html>';
  echo '<html>';
  echo '<head>';
  echo '<title>POST Method Test - Result</title>';
  echo '</head>';
  echo '<body>';
  echo '<h2>POST Method Test - Result</h2>';
  echo 'First Name: ' . $fname . '<br>';
  echo 'Last Name: ' . $lname . '<br>';

  echo '<h3>$_POST Dump:</h3>';
  echo '<pre>';
  var_dump($_POST);
  echo '</pre>';

  echo '</body>';
  echo '</html>';
}

?>
