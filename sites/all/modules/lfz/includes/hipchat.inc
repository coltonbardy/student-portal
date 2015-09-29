<?php

define("HIPCHAT_AUTH_TOKEN", "fF6MS1rOEpEoGQDpX6I2fwM1EPIzeFUCHwuuikDj");
define("HIPCHAT_AUTH_TOKEN_GLOBAL", "l2iUgRQqla7pu8os1166j1hTGFk4pg3EkyArz0RU");

function hipchat_request($api, $ags=array(), $auth_token=HIPCHAT_AUTH_TOKEN_GLOBAL, $decode=true){

    if(!isset($api)){
        return false;
    }

    $url = "https://api.hipchat.com/v2/".$api."/".join("/", $ags)."?auth_token=".$auth_token;

    $c = curl_init();
    curl_setopt($c, CURLOPT_URL, $url);
    curl_setopt($c, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($c, CURLOPT_SSL_VERIFYPEER, 0);
    $data = curl_exec($c);

    if($decode){
        $decoded_data = json_decode($data, true);
    }

    return ($decode && isset($decoded_data))?$decoded_data:$data;
}