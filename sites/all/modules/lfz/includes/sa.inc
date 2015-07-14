<?php

define("ANSWER_TYPE_BAD", "Bad Response");
define("ANSWER_TYPE_UNKNOWN", "User didn't know the answer");
define("ANSWER_TYPE_MORE", "More Corect Answers Needed");

function _parse_meta_data($meta_data_row)
{

    $meta_data = array();

    foreach ($meta_data_row as $value) {
        if (!strpos($value, "meta_data")) {
            $key_val_arr = explode(":", $value);
            $key = array_shift($key_val_arr);
            $meta_data[$key] = implode(":", $key_val_arr);
        }
    }

    return $meta_data;
}

function _column_data($data)
{

    $r = array();

    for ($i = 0; $i < count($data); $i++) {
        $row = $data[$i];
        foreach ($row as $key => $value) {
            if (!array_key_exists($key, $r)) {
                $r[$key] = array();
            }
            $r[$key][] = $value;
        }
    }

    return $r;
}

function _get_answers($data, $meta_data = array())
{

    $answers = array();
    if (count($meta_data) === 0) {
        return false;
    }

    $answer_row = ($meta_data['answer_key_index']) ? $meta_data['answer_key_index'] : 0;

    $i = 0;

    foreach ($data as $row) {
        if ($i == $answer_row) {
            $answers = $row;
        }

        $i++;
    }

    return $answers;
}

function _parse_response($data, $meta_data = array())
{

    $responses = array();
    if (count($meta_data) === 0) {
        return false;
    }

    for ($i = $meta_data['first_response_row']; $i < count($data); $i++) {
        //skip answer row
        if ($i == $meta_data['answer_key_index']) {
            continue;
        }

        if ($i >= $meta_data['first_response_row']) {
            $responses[] = $data[$i];
        }
    }

    return (count($responses) > 0) ? $responses : false;
}

function _format_user_data($responses, $data)
{

}

function _row_type($row)
{
    $rowTypeStr = "rowtype:";
    $type = false;
    foreach ($row as $value) {
        $pos = strpos($value, $rowTypeStr);
        if ($pos !== false) {
            $type = substr($value, $pos + strlen($rowTypeStr));
            break;
        }
    }

    return $type;
}

function _row_type_data($data)
{

    $r = array();

    foreach ($data as $row) {
        $type = _row_type($row);
        if ($type) {
            $r[$type] = array_values($row);
        }
    }
    
    return $r;
}

function _parse_question_types($data, $meta_data){

}

function _parse_questions($data, $meta_data){
    $r = array();

    $first_question_value = $meta_data['first_question_value'];

    $reached_first_question = false;

    $i = 0;
    foreach($data[0] as $key=>$value){

        if($key == $first_question_value){
            $reached_first_question = true;
        }

        if($reached_first_question && $key !=""){
            $r[] = array(
                "question"=>$key,
                "column_index"=>$i
            );
        }

        $i++;
    }

    return $r;
}

function _format_question_data($data, &$errors){

    $r = array();
    $row_type_data = _row_type_data($data);

    $meta_data = _parse_meta_data($row_type_data['meta_data']);
    if(!$meta_data){
        $errors[] = "Meta Data wasn't able to be parsed";
    }

    $answers = _get_answers($data, $meta_data);
    if(!$answers) {
        $errors[] = "Unable to determine answers row";
    }

    $questions = _parse_questions($data, $meta_data);

    $question_data_keys = array(
        'question_type',
        'associated_skill',
        'associated_topic',
        'incorrect_value_topic_association',
        'incorrect_value_impacted_skill'
    );

    foreach($questions as $question_arr){
        $col_index = $question_arr['column_index'];

        $answer = $answers[$question_arr['question']];

        //create temporary array to hold the formatted data
        $t_arr = array(
            "question"=>$question_arr['question'],
            "answer"=>$answer
        );

        foreach($question_data_keys as $key){
            $t_arr[$key] = $row_type_data[$key][$col_index];
        }

        //push temporary array to return array
        $r[] = $t_arr;
    }

    return $r;
}

function _parse_user_responses($data, &$errors){

    $r = array();
    $row_type_data = _row_type_data($data);

    $meta_data = _parse_meta_data($row_type_data['meta_data']);
    if(!$meta_data){
        $errors[] = "Meta Data wasn't able to be parsed";
    }

    $questions_data = _format_question_data($data, $errors);
    
    $responses =  _parse_response($data, $meta_data);
    if(!$responses){
        $errors[] = "No responses could be parsed from the data received";
    }
    
    foreach($responses as $response){
        $response_unique_id = $response[$meta_data['unique_student_identifier_key']];

        $user = _load_user_by_email($response_unique_id);

        echo '<pre>';
        print_r($user);
        echo '</pre>';

        if($user){
            $temp_user_arr = array();

            $temp_user_arr['uid'] = $user->uid;
            $temp_user_arr['questions'] = array();
            $temp_user_arr['data'] = array(
                'total_possible'=>0,
                'total_answered'=>0,
                'unknown_answered'=>0,
                'total_correct'=>0,
                'suggested_improvement'=>array(
//                    array(
//                        "topic"=>"",
//                        "skill"=>""
//                    ),
                )
            );

            foreach($questions_data as $q){
                $temp_response = array();

                $temp_response['question'] = $q['question'];
                $temp_response['response'] = $response[$q['question']];

                $options = array(
                    'type'=>$q['question_type'],
                    'unknown_answer_val'=>$meta_data['unknown_answer_val']
                );
                $temp_response['details'] = _evaluate_user_response($q['answer'], $response[$q['question']], $options);
                $temp_response['answer'] = $q['answer'];
                $temp_response['correct'] = ($temp_response['details']['total_possible'] === $temp_response['details']['total_correct'])?true:false;

                $temp_user_arr['questions'][] = $temp_response;



                $temp_user_arr['data']['total_possible'] += $temp_response['details']['total_possible'];
                $temp_user_arr['data']['total_answered'] += $temp_response['details']['total_answered'];
                $temp_user_arr['data']['total_correct'] += $temp_response['details']['total_correct'];
                $temp_user_arr['data']['unknown_answered'] += $temp_response['details']['unknown_answered'];

                if(!$temp_response['correct']){
                    $temp_user_arr['data']['suggested_improvement'] = array();
                    if(isset($q['associated_topic']) && $q['associated_topic'] != ""){
                        $temp_improvement_arr = array();
                        $temp_improvement_arr['topic'] = $q['associated_topic'];
                        $temp_improvement_arr['skill'] = $q['associated_skill'];
                        $temp_user_arr['data']['suggested_improvement'][] = $temp_improvement_arr;
                    }
                }
            }

            $r[$user->uid] = $temp_user_arr;
        }else{
            $errors[] = "User doesn't exist with : ".$response_unique_id;
        }

    }

    return $r;

}

function _evaluate_user_response($answer, $response, $options=array()){
    /**
     * Incorrect details breakdown
     * array of below
     *  - type : [ANSWER_TYPE_BAD | ANSWER_TYPE_UNKNOWN | ANSWER_TYPE_MORE | ANSWER_TYPE_DOES]
    */

    if(!isset($options['type'])){
        $options['type'] = "mc";
    }


    $r = array(
        'total_possible'=>1,
        'total_correct'=>0,
        'total_answered'=>0,
        'unknown_answered'=>0,
        'incorrect_details'=>array(
//            array(
//                'type'=>ANSWER_TYPE_BAD,
//                'response'=>''
//            )
        )
    );

    $unknown_answer = (isset($options['unknown_answer_val']))?trim(strtolower($options['unknown_answer_val'])):false;
    $answer = trim(strtolower($answer));
    $response = trim(strtolower($response));

    echo '<pre>';
    var_dump($response);
    echo '</pre>';

    switch($options['type']){
        case 'mc':
            if($unknown_answer
                && $unknown_answer === $response){
                $r['incorrect_details'][] = array(
                    'type'=>ANSWER_TYPE_UNKNOWN,
                    'response'=>$response
                );
                $r['unknown_answered'] = 1;
            }else if($answer === $response){
                $r['total_correct'] = 1;
                $r['total_answered'] = 1;
            }else{
                $r['incorrect_details'][] = array(
                    'type'=>ANSWER_TYPE_BAD,
                    'response'=>$response
                );
                $r['total_answered'] = 1;
            }
            break;
        case 'ms':
            $answer_arr = explode(",", $answer);
            //remove white space from multiple selects
            foreach($answer_arr as $key=>$answer){
                $answer_arr[$key] = trim($answer);
            }
            $response_arr = explode(",", $response);

            $r['total_possible'] = count($answer_arr);

            if(count($answer_arr) > count($response_arr)){
                $r['incorrect_details'][] = array(
                    "type"=>ANSWER_TYPE_MORE,
                    "response"=>$response
                );
            }



            foreach($response_arr as $response) {

                //remove white space from responses
                $response = trim($response);

                if ($unknown_answer
                    && $unknown_answer === $response
                ) {
                    $r['incorrect_details'][] = array(
                        'type' => ANSWER_TYPE_UNKNOWN,
                        'response' => $response
                    );
                    $r['unknown_answered'] = 1;
                } else if (array_search($response, $answer_arr) !== false) {
                    $r['total_correct'] += 1;
                    $r['total_answered'] += 1;
                } else {
                    $r['incorrect_details'][] = array(
                        'type' => ANSWER_TYPE_BAD,
                        'response' => $response
                    );
                    $r['total_answered'] += 1;
                }
            }

            break;
    }

    return $r;
}