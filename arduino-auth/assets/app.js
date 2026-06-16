//Setup websocket (Connection for arduino)
const socket = io(`http://${window.location.host}`);

//Wait for page elements to load
document.addEventListener('DOMContentLoaded', () =>{


  //Get all ui elements
  //
  const loginSection = document.getElementById('loginSection');
  const usernameInput = document.getElementById('usernameInput');
  const passwordInput = document.getElementById('passwordInput');
  const loginBtn = document.getElementById('loginBtn');
  
  const authSection = document.getElementById('authSection');
  const codeInput = document.getElementById('codeInput');
  const submitBtn = document.getElementById('submitBtn');

  const statusSection = document.getElementById('statusSection');
  const statusText = document.getElementById('statusText');

  loginBtn.addEventListener('click', generateCodeClick);
  submitBtn.addEventListener('click', submitCodeClick);

  //Python -> JS Messages

  socket.on("awaiting_code", onAwaitingCode);
  
  socket.on("access_granted", onAccessGranted);
  
  socket.on("access_denied", onAccessDenied);

  socket.on("lock_auth", onLockedAuth);
  
});

//If user logged in, send a request to Python to generate an auth code
function generateCodeClick(){

  if(usernameInput.value.trim() === "" || passwordInput.value.trim() === "")
  {
    statusText.textContent = "Username and Password are required";
    statusText.classList.remove("granted");
    statusText.classList.add("denied");
    return;
  }

  //Send emit to Python (POST)
  socket.emit('generate_code', {});
}

//Query the entered code in JS with the Python generated code
function submitCodeClick(){
  
  const enteredCode = codeInput.value;
  socket.emit("validate_code", {code: enteredCode});
}

//Styling displays for different states
//
function  onAwaitingCode(){

  statusText.textContent = "Enter the code shown on the device";
  statusText.classList.remove("granted", "denied");
  
  loginSection.style.display = "none";

  authSection.style.display = "block";

  submitBtn.style.display = "block";

  codeInput.value = "";
}

function onAccessGranted()
{
    loginSection.style.display = "none";

    authSection.style.display = "block";

    statusText.textContent = "ACCESS GRANTED";

    submitBtn.style.display = "none";

    statusText.classList.remove("denied");
    statusText.classList.add("granted");
}

function onAccessDenied()
{
  
    authSection.style.display = "none";

    loginSection.style.display = "block";

    statusText.textContent = "ACCESS DENIED";

    statusText.classList.remove("granted");
    statusText.classList.add("denied");
}

function onLockedAuth()
{
  statusText.textContent = "TOO MANY FAILED ATTEMPTS - LOCKED";

  authSection.style.display = "none";
  loginSection.style.display = "none";

  statusText.classList.remove("granted");
  statusText.classList.add("denied");
}

