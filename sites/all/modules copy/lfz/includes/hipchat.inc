<?php

define("HIPCHAT_AUTH_TOKEN_PORTAL", "f0uWcn2EYZk3EKmzf2q5648gL7WbZQk5vA3z2hjg");

function hipchat_request($api, $ags = array(), $request_body = false, $auth_token = HIPCHAT_AUTH_TOKEN_PORTAL, $decode = true)
{

    if (!isset($api)) {
        return false;
    }

    $url = "https://api.hipchat.com/v2/" . $api;
    if (count($ags) > 0) {
        $url .= "/" . join("/", $ags);
    }
    if (isset($auth_token)) {
        $url .= "?auth_token=" . $auth_token;
    }

    $c = curl_init();
    curl_setopt($c, CURLOPT_URL, $url);

    if ($request_body) {
        curl_setopt($c, CURLOPT_POST, 1);
        curl_setopt($c, CURLOPT_HTTPHEADER, array(
            'Content-Type: application/json'
        ));
        curl_setopt($c, CURLOPT_POSTFIELDS, json_encode($request_body));
    }

    curl_setopt($c, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($c, CURLOPT_SSL_VERIFYPEER, 0);
    $data = curl_exec($c);

    if ($decode) {
        $decoded_data = json_decode($data, true);
    }

    return ($decode && isset($decoded_data)) ? $decoded_data : $data;
}

function hipchat_send_message($message, $to)
{

    $message_details = array(
        "color" => "green",
        "message" => $message,
        "notify" => true,
        "message_format" => "html"
    );

    if ($to) {
        $user_message_response = hipchat_request('user', array($to, "message"), $message_details);

        if ($user_message_response != "") {
            watchdog(WATCHDOG_WARNING, $user_message_response);
            return false;
        }
    } else {
        return false;
    }

    return true;
}

function hipchat_key_words()
{
    return array(
        'agenda',
        'command',
        'css',
        'dan',
        'daniel',
        'docs',
        'drive',
        'Eric',
        'git',
        'github',
        'hipchat',
        'html',
        'instructor',
        'jordan',
        'jquery',
        'js',
        'php',
        'phpstorm',
        'portal',
        'project',
        'protocol',
        'prototype',
        'sa',
        'scott',
        'skill assessment',
        'terminal',
        'topic',
        'travis',
        'workflow'
    );
}

function search_message_for_key_words($message, $match_count=6){

    $keywords = hipchat_key_words();

    $match = array();
    $matches = array();

    for($i=0; $i<count($keywords); $i++){

        //check for match count and break out of loop if max has been found
        if(count($matches)>=$match_count){
            break;
        }
        $word = $keywords[$i];
        $regex = "/\b".$word."\b/i";

        preg_match($regex, $message, $match);

        if(count($match)>0){
            $matches[] = $match[0];
        }


    }

    return $matches;
}