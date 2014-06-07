// Indicate load success.
function moduleDidLoad() {

  // Send a message to the Native Client module
//common.hideModule(); 
  common.naclModule.postMessage('Hello from app.js'); 
}

// The 'message' event handler.  This handler is fired when the NaCl module
// posts a message to the browser by calling PPB_Messaging.PostMessage()
// (in C) or pp::Instance.PostMessage() (in C++).  This implementation
// simply displays the content of the message in an alert panel.
function handleMessage(message_event) {
  //alert(message_event.data);
  //console.log(message_event.data)
  var logEl = document.getElementById('log');
  logEl.innerHTML += "<p>" + message_event.data + "</p>";
  
}

