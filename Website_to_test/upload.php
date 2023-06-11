<?php 
echo "hello"; 
if (isset($_POST))
	print_r($_POST);

if (isset($_POST['try']))
	echo $_POST['try'];

?>
<!DOCTYPE html>
<html lang="tr">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
	<form method="POST" action="#">
	<input name="try" type="text">
	<input type="submit" value="send">
	</form>
</body>
</html>