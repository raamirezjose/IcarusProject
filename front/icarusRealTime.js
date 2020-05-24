
const socket = io();

function example(dir){
 socket.emit('postroutemap',{
	"direction":dir,
	"time":3.3
}); 
}

/* function enviar(){
    var text = document.getElementById("txt").value;
    socket.emit('chat:message',{message:text});
} */