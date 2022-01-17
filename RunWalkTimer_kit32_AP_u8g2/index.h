
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
  <style>
  h2 {
    color:  blue;
    font-family:  Arial;
    text-align:  Center;
    font-size:  200%;  
  }
  
  h3 {
    color:  blue;
    font-family:  Arial;
    text-align:  Center;
    font-size:  100%;  
  }
  </style
</head>

<body>

<h2>Run / Walk Pacer<h2>
<h3> Update Settings Here</h3>

<center>
<form action="/action_page" target="myIframe">
  Running time (sec):<br>
  <input type="number" name="runTime" value="60">
  <br>
  Walking time (sec):<br>
  <input type="number" name="walkTime" value="120">
  <br><br>
  <input type="radio" name="tone" value="on"> ToneOn<br>
  <input type="radio" name="tone" value="off"> ToneOff<br>
  <input type="submit" value="Submit">
</form> 

Run/Walk Value:<iframe name="myIframe" width="200" height="25" frameBorder="0"></iframe>

</center>
</body>
</html>
)=====";
