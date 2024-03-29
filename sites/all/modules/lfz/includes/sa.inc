<?php

define("ANSWER_TYPE_BAD", "Bad Response");
define("ANSWER_TYPE_UNKNOWN", "User didn't know the answer");
define("ANSWER_TYPE_MORE", "More Corect Answers Needed");
define("ANSWER_TYPE_MORE_RESPONSES", "User checked more answers than were needed");

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

function _parse_question_types($data, $meta_data)
{

}

function _parse_questions($data, $meta_data, &$errors)
{
    $r = array();

    $first_question_value = $meta_data['first_question_value'];

    $reached_first_question = false;

    $i = 0;
    foreach ($data[0] as $key => $value) {

        if ($key == $first_question_value) {
            $reached_first_question = true;
        }


        if ($reached_first_question && $key != "") {
            $r[] = array(
                "question" => $key,
                "column_index" => $i
            );
        }

        $i++;
    }

    if (!$reached_first_question) {
        $errors[] = "First question meta data doesn't match questions asked";
    }

    return $r;
}

function _format_question_data($data, &$errors)
{

    $r = array();
    $row_type_data = _row_type_data($data);

    $meta_data = _parse_meta_data($row_type_data['meta_data']);
    if (!$meta_data) {
        $errors[] = "Meta Data wasn't able to be parsed";
    }

    $answers = _get_answers($data, $meta_data);
    if (!$answers) {
        $errors[] = "Unable to determine answers row";
    }

    $questions = _parse_questions($data, $meta_data, $errors);

    $question_data_keys = array(
        'question_type',
        'associated_skill',
        'associated_topic',
        'incorrect_value_topic_association',
        'incorrect_value_impacted_skill'
    );

    foreach ($questions as $question_arr) {
        $col_index = $question_arr['column_index'];

        $answer = $answers[$question_arr['question']];

        //create temporary array to hold the formatted data
        $t_arr = array(
            "question" => $question_arr['question'],
            "answer" => $answer
        );

        foreach ($question_data_keys as $key) {
            $t_arr[$key] = $row_type_data[$key][$col_index];
        }

        //push temporary array to return array
        $r[] = $t_arr;
    }

    return $r;
}

function _parse_user_responses($data, &$errors)
{

    $r = array();
    $row_type_data = _row_type_data($data);

    $meta_data = _parse_meta_data($row_type_data['meta_data']);
    if (!$meta_data) {
        $errors[] = "Meta Data wasn't able to be parsed";
    }

    $questions_data = _format_question_data($data, $errors);

    $responses = _parse_response($data, $meta_data);
    if (!$responses) {
        $errors[] = "No responses could be parsed from the data received";
    }

    foreach ($responses as $response) {
        $response_unique_id = $response[$meta_data['unique_student_identifier_key']];

        $user = _load_user_by_email($response_unique_id);

        if ($user) {
            $temp_user_arr = array();

            $temp_user_arr['uid'] = $user->uid;
            $temp_user_arr['questions'] = array();
            $temp_user_arr['data'] = array(
                'total_possible' => 0,
                'total_answered' => 0,
                'unknown_answered' => 0,
                'total_correct' => 0,
                'suggested_improvement' => array(
                    'topics' => array(),
                    'skills' => array(),
                )
            );

            foreach ($questions_data as $q) {
                $temp_response = array();

                $temp_response['question'] = $q['question'];
                $temp_response['response'] = $response[$q['question']];

                $options = array(
                    'type' => $q['question_type'],
                    'unknown_answer_val' => $meta_data['unknown_answer_val']
                );
                $temp_response['details'] = _evaluate_user_response($q['answer'], $response[$q['question']], $options);
                $temp_response['answer'] = $q['answer'];
                $temp_response['topic'] = $q['associated_topic'];
                $temp_response['skill'] = $q['associated_skill'];

                $temp_response['correct'] = ($temp_response['details']['total_possible'] === $temp_response['details']['total_correct']
                    && $temp_response['details']['total_answered'] === $temp_response['details']['total_possible']) ? true : false;

                $temp_user_arr['questions'][] = $temp_response;


                $temp_user_arr['data']['total_possible'] += $temp_response['details']['total_possible'];
                $temp_user_arr['data']['total_answered'] += $temp_response['details']['total_answered'];
                $temp_user_arr['data']['total_correct'] += $temp_response['details']['total_correct'];
                $temp_user_arr['data']['unknown_answered'] += $temp_response['details']['unknown_answered'];

                if (!$temp_response['correct']) {

                    $topic = (isset($q['associated_topic'])) ? trim($q['associated_topic']) : "";
                    $skill = (isset($q['associated_skill'])) ? trim($q['associated_skill']) : "";


                    if ($topic != ""
                        && _in_array_by_key_value($temp_user_arr['data']['suggested_improvement']['topics'], 'name', $topic) === false
                    ) {
                        $temp_user_arr['data']['suggested_improvement']['topics'][] = array(
                            'name' => $topic,
                            'question' => $q['question'],
                            'response' => $response[$q['question']],
                            'answer' => $q['answer']
                        );
                    }

                    //check that the skill is not empty and doesnt exist in the array
                    if ($skill != ""
                        && _in_array_by_key_value($temp_user_arr['data']['suggested_improvement']['skills'], 'name', $skill) === false
                    ) {
                        $temp_user_arr['data']['suggested_improvement']['skills'][] = array(
                            'name' => $skill,
                            'question' => $q['question'],
                            'response' => $response[$q['question']],
                            'answer' => $q['answer']
                        );
                    }
                }
            }

            $r[$user->uid] = $temp_user_arr;
        } else {
            $errors[] = "User doesn't exist with : " . $response_unique_id;
        }


    }

    return $r;

}

/**
 * checks a single level of an array for a key that has a specified value, non strict comparison
 * @param $arr
 * @param $key
 * @param $value
 * @return bool
 */
function _in_array_by_key_value($arr, $key, $value){
    $r = false;
    foreach($arr as $a){
        if($a[$key] == $value){
            $r = true;
            break;
        }
    }

    return $r;
}

function _evaluate_user_response($answer, $response, $options = array())
{
    /**
     * Incorrect details breakdown
     * array of below
     *  - type : [ANSWER_TYPE_BAD | ANSWER_TYPE_UNKNOWN | ANSWER_TYPE_MORE | ANSWER_TYPE_DOES]
     */

    if (!isset($options['type'])) {
        $options['type'] = "mc";
    }


    $r = array(
        'total_possible' => 1,
        'total_correct' => 0,
        'total_answered' => 0,
        'unknown_answered' => 0,
        'incorrect_details' => array(
//            array(
//                'type'=>ANSWER_TYPE_BAD,
//                'response'=>''
//            )
        )
    );

    $unknown_answer = (isset($options['unknown_answer_val'])) ? trim(strtolower($options['unknown_answer_val'])) : false;
    $answer = trim(strtolower($answer));
    $response = trim(strtolower($response));

    switch ($options['type']) {
        case 'mc':
            if ($unknown_answer
                && $unknown_answer === $response
            ) {
                $r['incorrect_details'][] = array(
                    'type' => ANSWER_TYPE_UNKNOWN,
                    'response' => $response
                );
                $r['unknown_answered'] = 1;
            } else if ($answer === $response) {
                $r['total_correct'] = 1;
                $r['total_answered'] = 1;
                $r['correct_details'][] = array(
                    'type' => ANSWER_TYPE_UNKNOWN,
                    'response' => $response
                );
            } else {
                $r['incorrect_details'][] = array(
                    'type' => ANSWER_TYPE_BAD,
                    'response' => $response
                );
                $r['total_answered'] = 1;
            }
            break;
        case 'ms':
            $answer_arr = explode(",", $answer);

            //remove white space from multiple selects
            foreach ($answer_arr as $key => $answer) {
                $answer_arr[$key] = trim($answer);
            }
            $response_arr = explode(",", $response);

            $r['total_possible'] = count($answer_arr);

            if (count($answer_arr) > count($response_arr)) {
                $r['incorrect_details'][] = array(
                    "type" => ANSWER_TYPE_MORE,
                    "response" => $response
                );
            }else if(count($answer_arr) < count($response_arr)){
                $r['incorrect_details'][] = array(
                    "type" => ANSWER_TYPE_MORE_RESPONSES,
                    "response" => $response
                );
            }


            foreach ($response_arr as $response) {

                //remove white space from responses
                $response = trim($response);

                if ($unknown_answer
                    && $unknown_answer === $response
                ) {
                    $r['incorrect_details'][] = array(
                        'type' => ANSWER_TYPE_UNKNOWN,
                        'response' => $response
                    );
                    $r['unknown_answered'] += 1;
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

function _get_assessment_data($class_nid=false,$assessment_nid = false)
{

    global $user;

    $values = array();
    $r = array();

    //get all assessment data
    if (!$assessment_nid) {

        $sql = "";
        //get all assessment results for instructors
        if (user_access("manage skill assessments")) {
            if($class_nid){
                $sql = "SELECT nid, title, changed FROM {node}
                        LEFT JOIN {field_data_field_class_reference} field_data_field_class_reference ON node.nid = field_data_field_class_reference.entity_id AND (field_data_field_class_reference.entity_type = 'node' AND field_data_field_class_reference.deleted = '0')
                        WHERE type=:type
                            AND (field_data_field_class_reference.field_class_reference_nid IN  (:class_nid))
                        ORDER BY changed DESC";
                $values[":class_nid"] = array($class_nid);
            }else{
                $sql = 'SELECT nid, title, changed FROM {node} WHERE type=:type ORDER BY changed DESC';
            }
        } else if (user_has_role(array_search('student', user_roles()))) {
            //get results for specific students roles

            if($class_nid){
                $sql = "SELECT node.title AS title, node.nid AS nid, node.changed AS changed, 'node' AS field_data_field_class_reference_node_entity_type
                    FROM
                    {node} node
                    LEFT JOIN {field_data_field_class_reference} field_data_field_class_reference ON node.nid = field_data_field_class_reference.entity_id AND (field_data_field_class_reference.entity_type = 'node' AND field_data_field_class_reference.deleted = '0')
                    LEFT JOIN {node} node_field_data_field_class_reference ON field_data_field_class_reference.field_class_reference_nid = node_field_data_field_class_reference.nid
                    LEFT JOIN {field_data_field_access_roles} node_field_data_field_class_reference__field_data_field_access_roles ON node_field_data_field_class_reference.nid = node_field_data_field_class_reference__field_data_field_access_roles.entity_id AND (node_field_data_field_class_reference__field_data_field_access_roles.entity_type = 'node' AND node_field_data_field_class_reference__field_data_field_access_roles.deleted = '0')
                    WHERE (( (node.status = '1')
                        AND (node.type=:type)
                      AND (node_field_data_field_class_reference__field_data_field_access_roles.field_access_roles_value IN (:roles)) ))
                      AND (field_data_field_class_reference.field_class_reference_nid IN  (:class_nid))
                    ORDER BY node.changed ASC";
                $values[":class_nid"] = array($class_nid);
            }else {
                $sql = "SELECT node.title AS title, node.nid AS nid, node.changed AS changed, 'node' AS field_data_field_class_reference_node_entity_type
                    FROM
                    {node} node
                    LEFT JOIN {field_data_field_class_reference} field_data_field_class_reference ON node.nid = field_data_field_class_reference.entity_id AND (field_data_field_class_reference.entity_type = 'node' AND field_data_field_class_reference.deleted = '0')
                    LEFT JOIN {node} node_field_data_field_class_reference ON field_data_field_class_reference.field_class_reference_nid = node_field_data_field_class_reference.nid
                    LEFT JOIN {field_data_field_access_roles} node_field_data_field_class_reference__field_data_field_access_roles ON node_field_data_field_class_reference.nid = node_field_data_field_class_reference__field_data_field_access_roles.entity_id AND (node_field_data_field_class_reference__field_data_field_access_roles.entity_type = 'node' AND node_field_data_field_class_reference__field_data_field_access_roles.deleted = '0')
                    WHERE (( (node.status = '1')
                        AND (node.type=:type)
                      AND (node_field_data_field_class_reference__field_data_field_access_roles.field_access_roles_value IN (:roles)) ))
                    ORDER BY node.changed ASC";
            }

            $values[":roles"] = array_keys($user->roles);
        }

        $values[':type'] = 'skill_assessment_results';

        if (strlen($sql) > 0) {
            $query = db_query($sql, $values);

            while ($result = $query->fetchAssoc()) {
                $r[] = $result;
            }
        }

    } else {
        $r[] = _load_node_basic($assessment_nid);
    }

    return $r;
}

function _get_results_data($nid)
{
    //create an error array to be passed along as reference for any errors that could happen
    $errors = array();
    $r = array(
        'nid' => $nid,
        'errors' => array(),
        'formatted' => array()
    );


    //include Reader CSV class for easy csv manipulation
    module_load_include('php', 'lfz', 'lib/EasyCSV/Reader');
    //load the current node data based on the url
    $node = node_load($nid);

    //results data pulled from the body of the Skill Assessment Result content type
    $csv_data = $node->body['und'][0]['value'];

    //create temp file for Reader class to load and process
    $temp_csv_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'lfz') . '/tmp/sa/' . $nid . '-results.csv';

    //put csv into a temp file for reading with library
    if (!file_put_contents($temp_csv_file, $csv_data)) {
        $r['errors'][] = "Unable to write temp csv file to read from : " . $temp_csv_file;
    }

    //load temp csv file
    // CSV format is based on the google doc here
    //
    $reader = new \EasyCSV\Reader($temp_csv_file);
    $lib_data = $reader->getAll();

    $r['formatted']['raw'] = $lib_data;

    //get csv row types
    $row_type_data = _row_type_data($lib_data);
    $r['formatted']['row_type'] = $row_type_data;

    $meta_data = _parse_meta_data($row_type_data['meta_data']);
    if (!$meta_data) {
        $r['errors'][] = "Meta Data wasn't able to be parsed";
    }

    $r['formatted']['meta_data'] = $meta_data;

    $responses = _parse_response($lib_data, $meta_data);
    if (!$responses) {
        $r['errors'][] = "No responses could be parsed from the data received";
    }

    $questions_data = _format_question_data($lib_data, $errors);
    $r['formatted']['questions'] = $questions_data;

    $user_responses = _parse_user_responses($lib_data, $errors);
    $r['formatted']['user'] = $user_responses;

    $r['errors'] = array_unique($errors);

    return $r;
}

function _get_overview_issue_topic_data_by_question($data)
{

    $issue_topic_data_by_question = array();

    foreach ($data as $d) {
        $sa_results = _get_results_data($d['nid']);

        $users = $sa_results['formatted']['user'];

        foreach ($users as $user) {

            foreach($user['questions'] as $question_data){

                if(trim($question_data['topic']) == ""){
                    continue;
                }

                $question_clean = htmlentities($question_data['question']);
                $answer_clean = htmlentities($question_data['answer']);
                $response_clean = htmlentities($question_data['response']);

                if(!isset($issue_topic_data_by_question[$question_data['topic']])){
                    $issue_topic_data_by_question[$question_data['topic']] = array(
                        'total_questions'=>0,
                        'total_correct'=>0,
                        'questions'=>array()
                    );
                }

                $issue_topic_data_by_question[$question_data['topic']]['total_questions'] += 1;
                if($question_data['correct']){
                    $issue_topic_data_by_question[$question_data['topic']]['total_correct'] += 1;
                }

                $issue_topic_data_by_question[$question_data['topic']]['percent'] = round($issue_topic_data_by_question[$question_data['topic']]['total_correct']/$issue_topic_data_by_question[$question_data['topic']]['total_questions']*100);

                $question_key = $d['nid'].$question_clean;

                if(!isset($issue_topic_data_by_question[$question_data['topic']]['questions'][$question_key])){
                    $issue_topic_data_by_question[$question_data['topic']]['questions'][$question_key] = array(
                        'sa_link'=>_get_sa_link_by_result_nid($d['nid']),
                        'question'=>$question_clean,
                        'answer'=>$answer_clean,
                        'user_responses'=>array()
                    );
                }

                if(!$question_data['correct']){
                    $issue_topic_data_by_question[$question_data['topic']]['questions'][$question_key]['user_responses'][$user['uid']] = array(
                        'user_info'=>_get_user_get_basic_info($user['uid']),
                        'response'=>$response_clean
                    );
                }

            }

        }
    }

    uasort($issue_topic_data_by_question, "sort_user_data_by_topic_correct_percentage");

    return $issue_topic_data_by_question;
}

function sort_user_data_by_topic_correct_percentage($a, $b)
{

    $a_total_questions = $a['total_questions'];

    $b_total_questions = $b['total_questions'];

    if ($a['percent'] == $b['percent']) {
        if($a_total_questions < $b_total_questions){
            return 1;
        }else if($a_total_questions > $b_total_questions){
            return -1;
        }else{
            return 0;
        }
    }

    return ($a['percent'] < $b['percent']) ? -1 : 1;
}

function _get_overview_issue_topic_data_by_user($data)
{

    $issue_topic_data_by_user = array();

    foreach ($data as $d) {
        $sa_results = _get_results_data($d['nid']);

        $users = $sa_results['formatted']['user'];

        foreach ($users as $user) {

            $uid = $user['uid'];

            if (!isset($issue_topic_data_by_user[$uid])) {
                $issue_topic_data_by_user[$uid] = array(
                    'user_info' => _get_user_get_basic_info($uid),
                    'topics' => array()
                );
            }

            foreach($user['questions'] as $question){

                //check if this topic has been created for the current user
                // if not then create the data structure
                if(!isset($issue_topic_data_by_user[$uid]['topics'][$question['topic']])){
                    $issue_topic_data_by_user[$uid]['topics'][$question['topic']] = array(
                        'title'=>$question['topic'],
                        'total_questions'=>0,
                        'total_correct'=>0,
                        'skill_assessments'=>array()
                    );
                }

                if(!isset($issue_topic_data_by_user[$uid]['topics'][$question['topic']]['skill_assessments'][$d['nid']])){
                    $issue_topic_data_by_user[$uid]['topics'][$question['topic']]['skill_assessments'][$d['nid']] = array(
                        'nid'=>$d['nid'],
                        'total_questions'=>0,
                        'total_correct'=>0,
                        'questions'=>array()
                    );
                }

                $issue_topic_data_by_user[$uid]['topics'][$question['topic']]['total_questions'] += 1;
                $issue_topic_data_by_user[$uid]['topics'][$question['topic']]['skill_assessments'][$d['nid']]['total_questions'] += 1;
                if($question['correct']){
                    $issue_topic_data_by_user[$uid]['topics'][$question['topic']]['total_correct'] += 1;
                    $issue_topic_data_by_user[$uid]['topics'][$question['topic']]['skill_assessments'][$d['nid']]['total_correct'] += 1;
                }

                $issue_topic_data_by_user[$uid]['topics'][$question['topic']]['percent'] = round($issue_topic_data_by_user[$uid]['topics'][$question['topic']]['total_correct']/$issue_topic_data_by_user[$uid]['topics'][$question['topic']]['total_questions']*100);

                $issue_topic_data_by_user[$uid]['topics'][$question['topic']]['skill_assessments'][$d['nid']]['questions'][] = $question;

            }

//            uasort($topics_array, "sort_user_data_by_topic_correct_percentage");

        }

    }


    return $issue_topic_data_by_user;
}