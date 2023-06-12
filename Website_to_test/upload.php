<?php

// Get data from fields
$fname = $_SERVER['QUERY_STRING']['fname'];

echo '<!DOCTYPE html>';
echo '<html>';
echo '<head>';
echo '<title>Hello GET - CGI Program</title>';
echo '</head>';
echo '<body>';
echo '<h2>Hello ' . str_replace('+', ' ', $fname) . ' ' . str_replace('+', ' ', $lname) . ' ' . str_replace('+', ' ', $second) . ' ' . str_replace('+', ' ', $Third) . '</h2>';
echo '</body>';
echo '</html>';

?>
<!DOCTYPE html>
<html lang="tr">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
	<form method="post" action="">
	<input name="try" id="try" type="text">
	<input type="submit" value="send">
	</form>
</body>
</html>

