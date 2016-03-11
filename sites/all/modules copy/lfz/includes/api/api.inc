<?php
/**
 * Created by PhpStorm.
 * User: ericjohnson
 * Date: 1/26/16
 * Time: 11:53 AM
 */

function lfz_api_output($data, $succsss=false, $exit=true){

    $r = array(
        "success"=>$succsss,
        "data"=>$data
    );

    drupal_json_output($r);

    if($exit){
        exit;
    }
}