function sayHello() {
    var name = prompt("Adınızı girin:");
    var message = "Merhaba, " + name + "!";
    document.getElementById("greeting").textContent = message;
}