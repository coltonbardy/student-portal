<?php
/**
 * Created by PhpStorm.
 * User: ericjohnson
 * Date: 1/26/16
 * Time: 11:51 AM
 */

function api_prototype_user_class_list(){

    _check_cross();

    //@TODO uid needs to be dynamic in some way with authentication
    $users_classes = _get_users_classes(19);

    lfz_api_output($users_classes, true);
}

function api_prototype_class_students($class_nid){

    _check_cross();

    $r = array();

    if(is_numeric($class_nid) && $class_nid > 0){
        $students = _class_get_students($class_nid);
        foreach($students as $student){
            $student_node = node_load($student->nid);
            $field_value = _get_field_value($student_node, 'field_github_profile');
            $user = _get_student_user($student->nid);

            if($field_value && !empty($user['name'])){
                $r[] = array(
                    "name"=>$user['name'],
                    "uid"=>$user['uid'],
                    "repoUrl"=>$field_value
                );
            }

        }
    }

    lfz_api_output($r, true);
}