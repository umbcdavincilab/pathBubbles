<?php

//always use this username, otherwise you can't get results
$username="liang"; 

//This is password for you.
$password="iTermPageForLiang";

//Put your gene symbol here
$symbol="gro";


$result = file_get_contents("https://biotm.cis.udel.edu/udelafc/getiTermPage.php?user=$username&pass=$password&gene=$symbol");

header( 'Location: '.$result );
die();

?>


//
Liang's exmaple
<?php
if (!empty($_GET['symbol'])){
   $symbol=$_GET['symbol']; 
}
else{
    $symbol="GRO";
}

//always use this username, otherwise you can't get results
$username="liang"; 

//This is password for you.
$password="iTermPageForLiang";

//Put your gene symbol here



$result = file_get_contents("https://biotm.cis.udel.edu/udelafc/getiTermPage.php?user=$username&pass=$password&gene=$symbol");

header( 'Location: '.$result );
die();

?>
[3:16:00 PM] Liang Sun: ---------------------------------------
[3:16:01 PM] Liang Sun:       $('#egiftLink').click(function(){
        var symbol = target.data('name');
        var egiftLink= 'egiftLink.php?symbol='+symbol;
        window.open(egiftLink);
        
      });