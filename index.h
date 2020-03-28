const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <meta http-equiv='content-type' content='text/html; charset=UTF-8'/>
    <title>ESP8266-Funke v2.0</title>
    <style>
      body { background-color: #ffc627; font-family: Arial, Helvetica, Sans-Serif; Color: #996515; }
      .single_on { height:100px; width:110px; background-color:#CC8E34; color:black; border:none; font-size: 20px; font-weight:bold; }
      .single_off { height:100px; width:110px; background-color:#A97142; color:black; border:none; font-size: 20px; font-weight:bold; }
      .mood_on { height:100px; width:220px; background-color:#CC8E34; color:black; border:none; font-size: 20px; font-weight:bold; }
      .mood_off { height:100px; width:220px; background-color:#A97142; color:black; border:none; font-size: 20px; font-weight:bold; }
      .h1 { font-size:50px; text-align:center; }
      .uptime { font-size:30px; text-align:center; }
      .cfont { font-size:40px; text-align:center; }
    </style>
  </head>
  <body>
    <h1 class='h1'/>433Mhz ESP8266 Funke</h1>
<table align='center' cellpadding='15' border='0'/>
<tr> <td> <p class='cfont'/>i_1::</p></td><td><a href='send_i1_on'/><button class='single_on'>ON</button></a><a href='send_i1_off'/><button class='single_off'>OFF</button></a> </td>
     <td> <p class='cfont'/>m1a::</p></td><td><a href='send_m1a_on'/><button class='single_on'>ON</button></a><a href='send_m1a_off'/><button class='single_off'>OFF</button></a> </td></tr>
<tr> <td> <p class='cfont'/>i_2::</p></td><td><a href='send_i2_on'/><button class='single_on'>ON</button></a><a href='send_i2_off'/><button class='single_off'>OFF</button></a> </td>
     <td> <p class='cfont'/>m1b::</p></td><td><a href='send_m1b_on'/><button class='single_on'>ON</button></a><a href='send_m1b_off'/><button class='single_off'>OFF</button></a> </td></tr>
<tr> <td> <p class='cfont'/>i_3::</p></td><td><a href='send_i3_on'/><button class='single_on'>ON</button></a><a href='send_i3_off'/><button class='single_off'>OFF</button></a> </td>
     <td> <p class='cfont'/>m1c::</p></td><td><a href='send_m1c_on'/><button class='single_on'>ON</button></a><a href='send_m1a_off'/><button class='single_off'>OFF</button></a> </td></tr>
<tr> <td> <p class='cfont'/>i_4::</p></td><td><a href='send_i4_on'/><button class='single_on'>ON</button></a><a href='send_i4_off'/><button class='single_off'>OFF</button></a> </td>
     <td> <p class='cfont'/>m2a::</p></td><td><a href='send_m2a_on'/><button class='single_on'>ON</button></a><a href='send_m2a_off'/><button class='single_off'>OFF</button></a> </td></tr>
<tr> <td> <p class='cfont'/>i_5::</p></td><td><a href='send_i5_on'/><button class='single_on'>ON</button></a><a href='send_i5_off'/><button class='single_off'>OFF</button></a> </td>
     <td> <p class='cfont'/>m2b::</p></td><td><a href='send_m2b_on'/><button class='single_on'>ON</button></a><a href='send_m2b_off'/><button class='single_off'>OFF</button></a> </td></tr>
<tr> <td> <p class='cfont'/>i_6::</p></td><td><a href='send_i6_on'/><button class='single_on'>ON</button></a><a href='send_i6_off'/><button class='single_off'>OFF</button></a> </td>
     <td> <p class='cfont'/>m2c::</p></td><td><a href='send_m2c_on'/><button class='single_on'>ON</button></a><a href='send_m2c_off'/><button class='single_off'>OFF</button></a> </td></tr>
<tr><td></td><td></td></tr>

<tr><td> <p class='cfont'/>MOOD::</p></td><td><a href='send_mood_on'/><button class='mood_on'>ON</button><td></td></a></td><td><a href='send_mood_off'/><button class='mood_off'>OFF</button></a> </tr>
<tr><td> <p class='cfont'/>WORK::</p></td><td><a href='send_work_on'/><button class='mood_on'>ON</button><td></td></a></td><td><a href='send_work_off'/><button class='mood_off'>OFF</button></a> </tr>
<tr><td> <p class='cfont'/>ALL::</p></td><td><a href='send_all_on'/><button class='mood_on'>ON</button><td></td></a></td><td><a href='send_all_off'/><button class='mood_off'>OFF</button></a> </tr>

    </body>
</html>
)=====";
