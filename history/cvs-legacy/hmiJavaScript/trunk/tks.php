<?php
// Basis-Funktionen bekanntgeben

//setting right charset for umlauts in HMI grafics
header("Content-Type: text/html; charset=Windows-1252");

$phpKSIncludePath = "../phpks/";
$phpKSIncludeFile = "phpks.inc.php";

if (!extension_loaded ('phpks')){
	echo "KS PHP library not loaded.";
	exit;
}elseif (is_file($phpKSIncludePath.$phpKSIncludeFile)){
	//found file in shared folder
	include($phpKSIncludePath.$phpKSIncludeFile);
}elseif (is_file($phpKSIncludeFile)){
	//found file in own folder
	include($phpKSIncludeFile);
}else{
	echo "KS PHP Helperfile not found: ".$phpKSIncludePath.$phpKSIncludeFile;
	exit;
}

// Parameter holen
function expQsData(&$data) {
    global $_GET;
    global $_POST;
    
    if(isset($_GET)) {
	while(list($k,$v)=each($_GET)) {
	    $data[$k] = $v;
	}
    }
    
    if(isset($_POST)) {
	while(list($k,$v)=each($_POST)) {
	    $data[$k] = $v;
	}
    }
}

DEFINE("TKS_PREFIX", "TksS-0000");

function getEP($data) {
    global $KSERR;

    $hs = str_replace(TKS_PREFIX, "", $data['obj']);
    $tmp = $data['path'];

    $ar1 = explode(" ", $tmp);
    $i = 0;
    $ar = array();
    for($j=0; $j<count($ar1); $j++) {
	$tmp = trim($ar1[$j]);
	if($tmp != "") {
	    $ar[$i++] = $tmp;
	}
    }
    $path = $ar[0];

    $svc = new svc_ksGetEP;
    $svc->hs   = $hs;
    $svc->path = $path;
    if(count($ar) > 1) {
	$svc->name_mask = $ar[1];
    }

    if(!$svc->update()) {
	$err = $svc->lastResult();
	$svc->clean;
	return TKS_PREFIX.'::'.$KSERR[$err]." Can't read children";
    }

    $anz = $svc->resultSize();
    $obj = new KsEngProps;

    $ret = "";
    for($i=0; $i<$anz; $i++) {
	$obj->insert($svc, $i);
	$ret .= "{".$obj->identifier."} ";
    }

    $svc->clean;
    
    return trim($ret);
}

function getVar($data) {
    global $KSERR;

    $hs = str_replace(TKS_PREFIX, "", $data['obj']);
    $paths = trim($data['path']);
    $paths =str_replace("{", "", $paths);
    $paths =str_replace("}", "", $paths);
    
    $ar1 = explode(" ", $paths);
    $ar = array();
    $anzVars = 0;
    for($i=0; $i<count($ar1); $i++) {
	$tmp = trim($ar1[$i]);
	if($tmp != "") {
	    $ar[$anzVars++] = $tmp;
	}
    }
	    
    $svc = new svc_ksGetVar;
    $svc->hs   = $hs;
    for($v=0; $v<$anzVars; $v++) {
	$svc->add( $ar[$v] );
    }

    if(!$svc->update()) {
	$err = $svc->lastResult();
	$svc->clean;
	return TKS_PREFIX.'::'.$KSERR[$err]." Can't read variable";
    }

    $val = new ksValue;

    $ret = "";
    for($v=0; $v<$anzVars; $v++) {
	$val->insert($svc, $v);
	$anz = $val->size;
	$ret .= "{";
	for($i=0; $i<$anz; $i++) {
	    $ret .= "{".$val->value($i)."} ";
	}
	$ret .= "} ";
    }
    
    $svc->clean;
    
    $ret = str_replace("} }", "}}", $ret);
    return trim($ret);
}

function getVarType($hs, $path, &$vt) {
    global $KSERR;

    $svc = new svc_ksGetVar;
    $svc->hs   = $hs;
    $svc->add( $path );

    if(!$svc->update()) {
	$err = $svc->lastResult();
	$svc->clean;
	return TKS_PREFIX.'::'.$KSERR[$err]." Can't read type of variable '".$path."'";
    }

    $val = new ksValue;

    $ret = "";
    $val->insert($svc, 0);
    $vt = $val->vt;

    $svc->clean;
    
    return "";
}

function setVar($data) {
    global $KSERR;

    $hs = str_replace(TKS_PREFIX, "", $data['obj']);
    $path = trim($data['path']);
    $valu = trim($data['val']);

    // Value als Array?
    $ar = explode("} {", $valu);
    if(count($ar) > 0) {
	$valu = array();
	$pos = 0;
	for($i=0; $i<count($ar); $i++) {
	    $tmp = $ar[$i];
	    $tmp = str_replace("{","",$tmp); 
	    $tmp = str_replace("}","",$tmp); 
	    $valu[$pos++] = trim($tmp);
	}
    }

    $vt = 0;
    $msg = getVarType($hs, $path, $vt);
    if($msg != "") {
	return $msg;
    }

    $svc = new svc_ksSetVar;
    $svc->hs   = $hs;
    $svc->add( $path, $valu, $vt );

    if(!$svc->update()) {
	$err = $svc->lastResult();
	$svc->clean;
	return TKS_PREFIX.'::'.$KSERR[$err]." Can't write variable";
    }

    $svc->clean;
    
    return "";
}

function getTksObj($data) {
    $h = $data['args'];

    return TKS_PREFIX.$h;
}

$data = array();
expQsData($data);

$ret = "";
$cmd = strtolower($data['cmd']);
switch($cmd) {
    case "getep":
	$ret = getEP($data);
	break;
    case "getvar":
	$ret = getVar($data);
	break;
    case "setvar":
	$ret = setVar($data);
	break;
    case "tks-server":
	$ret = getTksObj($data);
	break;
    case "destroy":
	break;
    default:
	$ret = "Unknown command '".$cmd."'";
}

print($ret);
flush();
exit;

?>