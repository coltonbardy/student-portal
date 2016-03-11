<?php

function instructor_time()
{

    $content = '<p>Booking time with instructor by providing your email address as the help code, each week you will have the ability to use your email address for a single booking</p>';
    $content .= '<iframe src="https://lfz-instructors.youcanbook.me/" width="100%" style="border:none; height:550px; overflow: hidden;" />';

    return $content;
}

function question_queue_instructor_resolve($queue_item_nid)
{
    question_queue_resolve($queue_item_nid, "student-queue");
}

function question_queue_resolve($queue_item_nid, $redirect_url = "my-queue")
{
    global $user;

    $node = node_load($queue_item_nid);

    if (isset($node->nid)
        && $node->type == "user_queue_item"
    ) {

        // user is a student not an instructor but isn't the user who opened the queue item
        if (user_has_role(array_search('student', user_roles()))
            && $user->uid != $node->uid
            && !user_has_role(array_search('instructor', user_roles()))
        ) {
            drupal_set_message("You cant resolve a question submitted by another Student", 'warning');
            drupal_goto("my-queue");
        }

        $node->field_queue_item_status = array(
            'und' => array(
                array(
                    'value' => 'complete'
                )
            )
        );

        if (!lfz_node_save($node)) {
            drupal_set_message("While attempting to resolve a queue item there was an error saving, please contact the development team.", 'warning');
        } else {
            drupal_set_message('Question "' . $node->title . '" has been resolved.', 'status');
        }
    } else {
        drupal_set_message("Can't resolve a queue item with an invalid Node id", 'warning');
    }
    drupal_goto($redirect_url);
}

function question_queue_open_question($queue_item_nid)
{

    global $user;

    $node = node_load($queue_item_nid);

    $hipchat_user_id = (isset($node->field_message_user_id['und'][0]['value'])) ? $node->field_message_user_id['und'][0]['value'] : false;

    if (isset($node->nid)
        && $node->type == "user_queue_item"
    ) {

        //check if there is an assigned array && if its set
        if (count($node->field_queue_item_assigned_user) > 0
            && isset($node->field_queue_item_assigned_user['und'][0]['uid'])
        ) {

            if ($node->field_queue_item_assigned_user['und'][0]['uid'] == $user->uid) {
                $redirect_url = "node/" . $node->nid;
                drupal_goto($redirect_url);
            } else {
                // set message and goto to the student queue
                drupal_set_message("This queue item has already been assigned to another user", 'warning');
                drupal_goto("student-queue");
            }

        } else {

            //set to the current logged in user and open it from waiting
            $node->field_queue_item_assigned_user = array(
                'und' => array(
                    array(
                        'uid' => $user->uid
                    )
                )
            );
            $node->field_queue_item_status = array(
                'und' => array(
                    array(
                        'value' => 'open'
                    )
                )
            );

            if (!lfz_node_save($node)) {
                drupal_set_message("While attempting to assign a user to the queue item there was an error saving, please contact the development team.", 'warning');

                drupal_goto("student-queue");
            } else {

                $instructor_user_name = _user_get_name($user->uid);

                $message_user_name = _user_get_name($node->uid);
                $question = ($node->field_queue_question && isset($node->field_queue_question['und'][0]['value'])) ? $node->field_queue_question['und'][0]['value'] : "";

                $auto_response_message = '<p>Hi ' . $message_user_name . ', <br/> your question <b>"' . $question . '"</b> that was inserted into the queue has been opened by <span style="color:green">' . $instructor_user_name . "</span>.</p>";
                $auto_response_message .= "<p>" . $instructor_user_name . " will be reaching out to you shortly, <b>there is no need to respond to this message</b></p>";

                if ($hipchat_user_id) {
                    if (!hipchat_send_message($auto_response_message, $hipchat_user_id)) {
                        drupal_set_message("We attempted to send a message to the user that submitted the question but we were unable", 'warning');
                    }
                } else {
                    drupal_set_message("Unable to pull a Hipchat user id from the original message, an automatic message wasn't sent out to the user", 'warning');
                }

                $redirect_url = "node/" . $node->nid;
                drupal_goto($redirect_url);
            }
        }
    } else {
        drupal_set_message("Can't answer a queue item with an invalid Node id", 'warning');
    }

    drupal_goto("student-queue");
}

function hipchat_api_question()
{

    $temp_message_file = DRUPAL_ROOT . '/' . drupal_get_path('module', 'lfz') . '/tmp/queue/test.json';

    //pull request body, aparently this works
    $message_body = file_get_contents("php://input");

    $is_test = (isset($_REQUEST['test'])) ? (bool)$_REQUEST['test'] : false;

    //LOCAL TESTING ONLY this can be ignored
    if (!$message_body
        && $is_test
    ) {
        $message_body = '{"event": "room_message", "item": {"message": {"date": "2015-09-23T03:12:08.182564+00:00", "from": {"id": 1848205, "links": {"self": "https://api.hipchat.com/v2/user/1848205"}, "mention_name": "EricJohnson", "name": "Eric Johnson", "version": "00000000"}, "id": "70e43903-9a77-4409-ad31-b12724fdf4d9", "mentions": [], "message": "/q help me please?", "type": "message"}, "room": {"id": 1299248, "links": {"members": "https://api.hipchat.com/v2/room/1299248/member", "participants": "https://api.hipchat.com/v2/room/1299248/participant", "self": "https://api.hipchat.com/v2/room/1299248", "webhooks": "https://api.hipchat.com/v2/room/1299248/webhook"}, "name": "Teaching", "version": "6THUMLP6"}}, "oauth_client_id": "3bf98ff9-b847-4ea0-9413-20959a08230d", "webhook_id": 2632464}';
    }


    //decode the body of the message string
    $data = json_decode($message_body, true);

    $temp_matches = array();

    $hipchat_user_id = $data['item']['message']['from']['id'];

    $message_user_data = hipchat_request("user", array($hipchat_user_id));

    //check if we are testing
    if (!$is_test) {
        //load user by email of hipchat user
        $user = user_load_by_mail($message_user_data['email']);
    } else {
        //use user with development email address
        $user = user_load_by_mail("development@learningfuze.com");
    }


    //insert data into watchdog table for debugging
    //watchdog(WATCHDOG_DEBUG, json_encode($data));

    //check that the event is a room message,
    // this would allow for multiple tips of web hooks to call the same url
    if ($data['event'] === 'room_message') {
        $message_string = $data['item']['message']['message'];
        $message_user = (isset($data['item']['message']['form']['name'])) ? $data['item']['message']['form']['name'] : false;

        //match against this type of string /q any string followed by a question mark? Topic Component URL
        preg_match_all("/\/q ([^?]+\?)[ ]*([^ ]*)[ ]*(.*)/", $message_string, $temp_matches);
    }

    $matches = array();

    //because empty matches are still grouped by () in the regex above then we have to loop through the array and pull
    // out none empty matches
    foreach ($temp_matches as $match) {
        if (!empty($match)) {
            $matches[] = $match;
        }
    }

    //check if there are more matches which would mean more than a question parameter after the /q
    if (count(array_values($matches)) >= 2) {
        $question = $matches[1][0];
    } else {
        //build out message response to a valid question asked
        $r = array(
            "color" => "red",
            "message" => "Message needs to be formatted like this. <b>/q sentence formatted like a question ending with a question mark?</b>",
            "notify" => false,
            "message_format" => "html"
        );

        drupal_json_output($r);
        exit;
    }

    $param1 = (count($matches[2]) > 0) ? trim($matches[2][0]) : false;
    $param2 = (count($matches[3]) > 0) ? trim($matches[3][0]) : false;

    $url = false;
    $category = false;

    if ($param1) {
        //checking to make sure its a category and not a url
        if (!is_valid_url($param1)) {
            $category = $param1;

            //check if there is a second parameter
            if (isset($param2)) {
                $url = $param2;
            }

        } else {
            $url = $param1;
        }
    }

    $body = $url;

    //node title is made up of the acroynm for the node type followed by keywords pulled from keyword list here hipchat_key_words & then a timestamp
    $message_keywords = search_message_for_key_words($question);
    $keywords_for_title = (count($message_keywords) > 0) ? " " . join(" ", $message_keywords) . " -" : "";
    $node_title = 'uqi -' . $keywords_for_title . " " . time();

    $user_queue_item = array(
        'type' => 'user_queue_item',
        'title' => $node_title,
        'field_queue_question' => array(
            'und' => array(
                array(
                    'value' => $question
                )
            )
        ),
        'body' => array(
            'und' => array(
                array(
                    'value' => $body,
                    'summary' => NULL,
                    'format' => NULL,
                    'safe_value' => htmlentities($body),
                    'safe_summary' => NULL
                )
            )
        ),
        'uid' => $user->uid,
        'field_user_queue_item_category' => array(
            'und' => array(
                array(
                    'value' => $category,
                    'format' => NULL,
                    'safe_value' => htmlentities($category)
                )
            )
        ),
        'field_queue_item_status' => array(
            'und' => array(
                array(
                    'value' => 'waiting'
                )
            )
        ),
        'field_message_user_id' => array(
            'und' => array(
                array(
                    'value' => $hipchat_user_id
                )
            )
        )
    );

    //create a new queue node from the question
    $new_node = _create_node($user_queue_item);

    if ($new_node) {
        $google_search_link = string_to_google_search_link($question);

        //check for assigned user
        $assigned_uid = variable_get("question_queue_assignee", false);
        $assigned_user = "An instructor"; // default text when there is no assigned user
        if ($assigned_uid) {
            $assigned_user_arr = _get_user_get_basic_info($assigned_uid);
            $assigned_user = $assigned_user_arr['name'];
        }

        date_default_timezone_set('America/Los_Angeles');

        $h = date("Gi");
        $day_of_week = date("D");
        /**
         * possible statues
         * -1 = off duty
         * 0 = on duty before class hours
         * 1 on duty during class hours
         * 2 on duty after class hours
         */
        $duty_status = 1;

        //check if message is on the weekend
        if (in_array($day_of_week, array("Sat", "Sun")) == -1) {
            $duty_status = -1;
        } else {
            if ($h < 1000 && $h > 830) {
                $duty_status = 0;
            } else if ($h > 1800 && $h < 2030) {
                $duty_status = 2;
            }
        }


        $r = array(
            "color" => "green",
            "message" => theme('question_queue_auto_response', array('assigned_user' => $assigned_user, 'google_search_link' => $google_search_link, 'duty_status' => $duty_status)),
            "notify" => false,
            "message_format" => "html"
        );
    } else {
        $r = array(
            "color" => "red",
            "message" => "There was an issue saving the question to the portal, An instructor will be informed and follow up with your question shortly.",
            "notify" => false,
            "message_format" => "html"
        );
    }

    drupal_json_output($r);
}

function question_queue_assign()
{
    global $user;
    $uid = (isset($_REQUEST['uid'])) ? $_REQUEST['uid'] : $user->uid;

    $is_set = variable_set("question_queue_assignee", $uid);
    $output = array(
        "data" => NULL,
        "success" => true
    );

    drupal_json_output($output);
}

function question_queue_unassign()
{
    global $user;
    $uid = (isset($_REQUEST['uid'])) ? $_REQUEST['uid'] : $user->uid;

    $assigned_uid = variable_get("question_queue_assignee", false);
    $output = array(
        "data" => NULL,
        "success" => true
    );

    if ($assigned_uid == $uid) {
        variable_set("question_queue_assignee", false);
    } else {
        $output['success'] = false;
    }


    drupal_json_output($output);
}

function ajax_remove_agenda_item_callback()
{

    $agenda_nid = $_REQUEST['agenda_nid'];
    $item_nid = $_REQUEST['item_nid'];
    $index = $_REQUEST['index'];
    $date = $_REQUEST['date'];
    $class_nid = $_REQUEST['class_nid'];

    if (!$agenda_nid && $date) {
        $agenda_node = get_daily_agenda($date, $class_nid);
    }
    $agenda_node = node_load($agenda_nid);

    $res = array('success' => true, 'data' => null);

    $did_remove = _remove_agenda_item_from_agenda($agenda_node, $item_nid, $index);

    if (!is_array($did_remove) && $did_remove) {
        $res['data'] = $agenda_node;
    } else {
        $res['success'] = false;
        $res['data'] = $did_remove[1];
    }

    drupal_json_output($res);
}

function ajax_agenda_item_save()
{
    $item_nid = $_REQUEST['item_nid'];
    $current_agenda_id = $_REQUEST['current_agenda_id'];
    $new_agenda_id = $_REQUEST['new_agenda_id'];
    $new_index = $_REQUEST['new_index'];
    $current_index = $_REQUEST['current_index'];
    $agenda_date = $_REQUEST['agenda_date'];
    $clone = (bool)$_REQUEST['clone'];
    $date = $_REQUEST['agenda_date'];
    $class_nid = $_REQUEST['class_nid'];

    $res = array('success' => false, 'data' => NULL, 'content' => '');

    //no agenda exists
    if (!$new_agenda_id) {
        if ($date) {
            //create new agenda
            $agenda_node = _create_agenda_by_date($date, $class_nid);
            if (!$agenda_node) {
                $res['content'] = 'Unable to create an agenda based on the date : ' . $date;
                echo json_encode($res);
                exit;
            }
            $new_agenda_id = $agenda_node->nid;
        } else {
            $res['content'] = 'No agenda id or date to create new agenda';
            echo json_encode($res);
            exit;
        }
    } else {
        $agenda_node = node_load($new_agenda_id);
    }

    if ($agenda_node) {
        //agenda exists so we can add the item or remove the old items
        if ($new_agenda_id != $current_agenda_id) {
            //item has been moved to a new agenda ndoe
            $remove_from_agenda = $current_agenda_id;
        } else {
            //its not a new agenda item so we use the agenda node
            $remove_from_agenda = $agenda_node;
        }

        //check if the item is a clone
        if (!$clone) {
            $did_remove = _remove_agenda_item_from_agenda($remove_from_agenda, $item_nid, $current_index);
            //old agenda remove index to add it back in
            if (is_array($did_remove) && !$did_remove[0]) {

                $res['content'] = 'Unable to remove agenda item:' . $item_nid . ' from agenda:' . $remove_from_agenda->nid;
                echo json_encode($res);
                exit;
            }
        }

        //add to item to the new agenda
        if (_add_item_to_agenda($agenda_node, $item_nid, $new_index)) {
            $res['success'] = true;
        } else {
            //item wasn't added
            $res['content'] = 'Error adding item:' . $item_nid . ' to agenda nid : ' . $agenda_node->nid;
        }

    } else {
        $res['content'] = 'Error loading node data from nid : ' . $new_agenda_id;
    }

    echo json_encode($res);
    exit;
}

function ajax_agenda_item_search_callback($search_val)
{

    $content = theme('bootstrap_list_item',
        array(
            'content' => 'Create New Item - ' . $search_val,
            'attr' => array(
                'class' => 'list-group-item-success add-new-item',
                'data-search-val' => $search_val,
            ),
        )
    );

    $search_results = get_view_data('search/' . $search_val);

    foreach ($search_results['nodes'] as $result) {
        $list_item_content = theme('agenda_item_heading',
                array(
                    'nid' => $result['node']['nid'],
                    'title' => trim($result['node']['title']),
                    'type' => $result['node']['type'],
                    'links' => false,
                )
            ) . '<span class="clearfix"></span>';

        $content .= theme('bootstrap_list_item',
            array(
                'content' => $list_item_content,
                'attr' => array(
                    'class' => 'add-item-search-result',
                    'data-nid' => $result['node']['nid'],
                ),

            )
        );
    }

    echo json_encode(array('success' => true, 'content' => $content));
    exit;
}

function ajax_get_agenda_type_callback()
{

    //#TODO make the types dynamic
    $types = array('agenda', 'assignment', 'projects', 'resource', 'topics');
    $items = array();

    foreach ($types as $type) {
        $items[] = array('content' => $type, 'attr' => array('class' => 'agenda-item-types', 'data-type' => $type));
    }

    $content = theme('bootstrap_list_group', array('items' => $items));

    echo json_encode(array('success' => true, 'content' => $content));
    exit;
}

function ajax_add_new_agenda_item_callback($date = false)
{
    $title = $_REQUEST['title'];
    $type = $_REQUEST['type'];

    $r = array('success' => false, 'data' => 'Unable to create new node with title :' . $title, ', type:' . $type);

    $node = _create_node(array('title' => $title, 'type' => $type));

    //add newley created agenda item
    if (is_object($node) && $node->nid) {
        ajax_agenda_today_add_callback($date, $node->nid);
    } else {
        return $r;
    }
}

function ajax_agenda_today_add_callback($date = false, $nid = false, $class_nid = false)
{
    $agenda_item_nid = ($nid) ? $nid : $_REQUEST['nid'];
    $class_nid = ($class_nid) ? $class_nid : $_REQUEST['class_nid'];

    if (!isset($class_nid)) {
        //choose most relevant class
        $classes = _get_current_classes();

        if (count($classes) == 1) {
            $class_nid = $classes[0]->nid;
        } else {
            //this should error out
            $res['success'] = false;

            if (count($classes) == 0) {
                $res['content'] = "Don't have the ability to save agenda item without a class reference";
            } else {
                $res['content'] = $classes;
            }

            echo json_encode($res);
            exit;
        }
    }

    $agenda_node = get_daily_agenda($date, $class_nid);

    if (!$agenda_node) {
        //create agenda node
        $agenda_node = _create_agenda_by_date($date, $class_nid);
    }

    $res = array('success' => true, 'data' => null);

    if (_add_item_to_agenda($agenda_node, $agenda_item_nid)) {
        $agenda_item = node_load($agenda_item_nid);

        $obj = _prepare_agenda_item($agenda_item);
        $obj['attr'] = array(
            'data-index' => count($agenda_node->field_agenda_item['und']) - 1,
            'data-agenda-nid' => $agenda_node->nid,
        );

        $res['content'] = theme('agenda_item_list', $obj);
    } else {
        $res['success'] = false;
        $res['content'] = 'Error saving to node';
    }

    echo json_encode($res);
    exit;
}

function ajax_get_agenda_items_callback($date = false)
{
    $class_nid = ($_REQUEST && isset($_REQUEST['class_nid'])) ? $_REQUEST['class_nid'] : false;

    $agenda_node = get_daily_agenda($date, $class_nid);

    $res = array('success' => false, 'content' => null);

    $agenda_items = get_agenda_items_by_agenda($agenda_node);

    if ($agenda_items) {
        $res['success'] = true;

        foreach ($agenda_items as $key => $item) {
            $obj = _prepare_agenda_item($item);
            $obj['attr'] = array(
                'data-index' => $key,
                'data-agenda-nid' => $agenda_node->nid,
            );

            $res['content'] .= theme('agenda_item_list', $obj);
        }

    }

    echo json_encode($res);
    exit;
}

function weekly_agenda_callback($class_nid = false, $date = false)
{

    if (!$class_nid) {

        $redirect_url = 'class/{class_nid}/week';
        if ($date) {
            $redirect_url .= '/' . $date;
        }

        return _agenda_select_class($redirect_url);
    }

    $class = node_load($class_nid);

    //modify the Title
    $title = drupal_get_title() . " - " . $class->title;
    drupal_set_title(t($title));

    $week_data = thisWeekInfo($date);

    if (!$week_data['current_week']) {
        $friendly_title_dates = date('n/j/y', $week_data['week']['start_ts']) . ' - ' . date('n/j/y', $week_data['week']['end_ts']);
        $title = 'Weekly Agenda - ' . $friendly_title_dates;
        drupal_set_title($title);
    }

    //was unable to produce this sql statement through the views
    $sql = "SELECT node.title AS node_title, node.nid AS nid, node.type AS node_type, node.created AS node_created
			FROM {node} node
			LEFT JOIN {field_data_field_date} field_data_field_date
			ON node.nid = field_data_field_date.entity_id
				AND (field_data_field_date.entity_type = 'node'
				AND field_data_field_date.deleted = '0')
			LEFT JOIN {field_data_field_class_reference} field_data_field_class_reference ON node.nid = field_data_field_class_reference.entity_id AND (field_data_field_class_reference.entity_type = 'node' AND field_data_field_class_reference.deleted = '0')
			WHERE (( (DATE_FORMAT(FROM_UNIXTIME(field_data_field_date.field_date_value), '%Y-%m-%d') >= '" . $week_data['week']['start_date'] . "'
					AND DATE_FORMAT(FROM_UNIXTIME(field_data_field_date.field_date_value), '%Y-%m-%d') <= '" . $week_data['week']['end_date'] . "') )
				AND(( (node.status = '1')
				AND (node.type IN  ('agenda')) )))
				AND field_data_field_class_reference.field_class_reference_nid IN  ('" . $class_nid . "')
				ORDER BY node_created DESC";

    $result = db_query($sql);

    $data = $result->fetchAll();

    $week_results = array();

    foreach ($week_data['days'] as $day_of_week_key => $day) {
        $week_results[$day_of_week_key] = $day;
        $week_results[$day_of_week_key]['title'] = t($day_of_week_key);
        $week_results[$day_of_week_key]['items'] = array();
        $week_results[$day_of_week_key]['is_today'] = $day['is_today'];
        $week_results[$day_of_week_key]['attr'] = array();
        $week_results[$day_of_week_key]['attr']['class'] = ' sortable connected-sortable';
        $week_results[$day_of_week_key]['attr']['data-date'] = $day['date'];

        $url = 'class/' . $class_nid . '/today/' . $day['date'];

        $week_results[$day_of_week_key]['agenda_url'] = $url;
        $week_results[$day_of_week_key]['link'] = l($week_results[$day_of_week_key]['title'], $url, array('attributes' => array('target' => '_blank')));
    }

    $weekly_content = '';

    foreach ($data as $d) {

        $agenda = node_load($d->nid);
        $agenda_date_ts = $agenda->field_date['und'][0]['value'];
        $day_of_week_key = date('l', $agenda_date_ts);

        $items = get_agenda_items_by_agenda($agenda->nid);

        if ($items) {

            // //if items then add sortable
            // $week_results[$day_of_week_key]['attr']['class'] = ' sortable connected-sortable';
            $week_results[$day_of_week_key]['attr']['data-agenda-nid'] = $agenda->nid;

            $i = 0;
            foreach ($items as $item) {

                $obj = _prepare_agenda_item($item);
                $obj['attr'] = array(
                    'data-index' => count($agenda->field_agenda_item['und']) - 1,
                    'data-agenda-nid' => $agenda->nid,
                );
                $week_results[$day_of_week_key]['items'][] = $obj;

                $i++;
            }
        }
    }

    drupal_add_js(array('lfz' => array('class_nid' => $class_nid)),
        'setting');

    drupal_add_js(drupal_get_path('module', 'lfz') . "/misc/agenda.js");

    $t = ($date) ? strtotime($date) : time();
    $y_t = $t - 60 * 60 * 24 * 7;
    $t_t = $t + 60 * 60 * 24 * 7;
    $yesterday = date("Y-m-d", $y_t);
    $tomorrow = date("Y-m-d", $t_t);

    $last_week_link = l('<span class="glyphicon glyphicon-chevron-left"></span>Last Weeks Agenda', "class/" . $class_nid . "/week/" . $yesterday, array("html" => true));
    $next_week_link = l('Next Weeks Agenda<span class="glyphicon glyphicon-chevron-right"></span>', "class/" . $class_nid . "/week/" . $tomorrow, array("html" => true));


    return theme('weekly_agenda', array('days' => $week_results, 'last_week_link' => $last_week_link, 'next_week_link' => $next_week_link));
}

function todays_agenda_callback($class_nid = false, $date = false)
{

    if (!$class_nid) {

        $redirect_url = 'class/{class_nid}/today';
        if ($date) {
            $redirect_url .= '/' . $date;
        }

        return _agenda_select_class($redirect_url);
    }

    $class = node_load($class_nid);

    //modify the Title
    $title = drupal_get_title() . " - " . $class->title;
    drupal_set_title(t($title));

    //check for class node otherwise redirect
    if (!$class->nid) {
        drupal_goto('today');
    }

    $week_data = thisWeekInfo($date);
    //track todays timestampe
    if ($date) {
        //when a date is passed in update the timestamp used
        $title = date('D M jS Y', $week_data['date_ts']) . ' - Agenda';

        drupal_set_title($title);
    }

    //add settings for ajax searching
    drupal_add_js(array('lfz' => array('ts' => $week_data['date_ts'],
        'date_search_format' => $week_data['date'],
        'class_nid' => $class->nid)),
        'setting');

    $todays_agenda = _create_agenda_by_date($date, $class->nid);

    if (!$todays_agenda) {
        return '<div class="alert alert-danger">Error creating Agenda for today</div>';
    }

    //get form that has the search/add feature and other html markup for
    // autocomplete and agenda list
    $return = drupal_get_form('lfz_agenda_form', $todays_agenda->nid, $class->nid, $date);

    //include js file
    drupal_add_js(drupal_get_path('module', 'lfz') . "/misc/agenda.js");
    drupal_add_js(drupal_get_path('module', 'lfz') . "/misc/todays-agenda.js");

    return $return;
}

function skill_assessment_details_callback($nid)
{

    global $user;

    $results = _get_results_data($nid);

    //currently print out errors, should be themed
    if (count($results['errors']) > 0) {
        foreach ($results['errors'] as $error) {
            drupal_set_message($error, "warning");
        }
    }

    $theme_data = array();

    //show all users for instructor role users
    if (user_has_role(array_search('instructor', user_roles()))) {
        //loop through each response and send the data to the template
        $theme_data = array("data" => $results['formatted']['user']);
        //only show the current student data if the user has a role of student
    } else if (user_has_role(array_search('student', user_roles()))
        && isset($results['formatted']['user'][$user->uid])
    ) {
        $student_data = $results['formatted']['user'][$user->uid];
        $theme_data = array("data" => array($student_data));
    } else {
        $theme_data = array("data" => array());
    }

    $r = theme("skill_assessment_user_list", $theme_data);

    return $r;
}

function skill_assessment_list_callback()
{

    global $user;

    drupal_set_title("Skill Assessments Taken");

    $data = _get_assessment_data();

    $r = theme("skill_assessment_list", array("data" => $data));

    if (count($data) == 0) {
        $r = "No Results for this page";
    }


    return $r;
}

function skill_assessment_save_results_callback()
{

    $data = $_REQUEST['data'];
    $nid = $_REQUEST['resource_nid'];
    $agenda_nid = $_REQUEST['agenda_nid'];
    $agenda_node = node_load($agenda_nid);
    $class_ref = $agenda_node->field_class_reference;
    $sa_arr = _load_node_basic($nid);

    $res = array();
    $res['success'] = false;
    $res['content'] = "Error saving results of skill assessment";

    if (isset($sa_arr['title']) && isset($class_ref)) {
        $new_node = array(
            'title' => ucwords($sa_arr['title'] . ' results'),
            'type' => 'skill_assessment_results',
            'body' => array(
                'und' => array(
                    array('value' => $data)
                )
            ),
            'field_class_reference' => array(
                'und' => array(
                    array('nid' => $class_ref['und'][0]['nid'])
                )
            ),
            'field_related_resources' => array(
                'und' => array(
                    array('nid' => $nid)
                )
            )
        );

        $create_results = _create_node($new_node);
        if ($create_results) {
            $res['success'] = true;
            $res['content'] = "Saved skill assessment results";
        }
    }

    echo json_encode($res);
    exit;
}

function skill_assessment_overview_callback($class_nid = false)
{

    //check for class nid ortherwise give a form for use to select the class
    if (!$class_nid) {
        $redirect_url = 'skillassessment/overview/{class_nid}';

        return _agenda_select_class($redirect_url);
    }

    $class_node = node_load($class_nid);

    drupal_set_title($class_node->title . " Assessment Overview");

    $data = _get_assessment_data();

    $template_data = array();

    $template_data['topic_by_question'] = _get_overview_issue_topic_data_by_question($data);
    $template_data['topic_by_user'] = _get_overview_issue_topic_data_by_user($data);

//    $r = theme('skill_assessment_overview', array('data'=>$template_data));
    $r = theme('skill_assessment_topic_issues_list', array('data' => $template_data));

    return $r;
}


function lfz_update_callback()
{

    //select all user queue items and update to public
    $sql = "SELECT node.nid AS nid FROM {node} node WHERE type='user_queue_item'";

    $result = db_query($sql, array(':type' => "user_queue_item"));

    $data = $result->fetchAll();

    foreach ($data as $obj) {
        $node = node_load($obj->nid);

        $node->field_public = array(
            "und" => array(
                array(
                    "value" => 1
                )
            )
        );

        lfz_node_save($node);
    }

}

function ajax_info_page_mail_callback()
{
    _check_cross();

    date_default_timezone_set('America/Los_Angeles');

    $data = (isset($_REQUEST) && !empty($_REQUEST)) ? $_REQUEST : false;

    $email_body = "";
    $to_email = "info@learningfuze.com";
    $r = array("success" => true, "message" => "Thank you for submitting your RSVP! Please check your email.");
    $email_params = array(
        "subject" => "Info Session Form Submitted",
        "body" => "Data sent to the Portal was malformed at " . date("m/d/Y H:i:s", time())
    );

    //@TODO create email template
    if ($data) {
        $email_body = "<h3>Info Session Form Submitted at " . date("m/d/Y H:i:s", time()) . "</h3>";
        foreach ($data as $key => $val) {
            $email_body .= "<p><b>" . $key . ": </b>" . $val . "</p>";
        }

        $email_body .= "<p>Portal Email System</p>";

        $email_params['body'] = $email_body;

    } else {
        $r['success'] = false;
        $r['message'] = "We were unable to process your information, a email has been sent to support@learningfuze.com. If you need further assistance please reach out via email at info@learningfuze.com";
    }

    //attempt sending the email
    $message = drupal_mail("lfz", "info_session", $to_email, user_preferred_language(null), $email_params);

    //check the rsult of the drupal system
    if (!isset($message['result']) || !$message['result']) {
        $r['success'] = false;
        $r['message'] = "Error sending mail, please contact support@learningfuze.com to finalize your RSVP.";
    }

    //keep track of successful requests attempts
    if ($r['success']) {
        $count = variable_get("info_page_request_count", 0);
        variable_set("info_page_request_count", $count + 1);
    }

    //add debug ability
    if (isset($data['debug']) && $data['debug']) {
        $r['debug'] = $message;
    }

    //@TODO information for a user should probably be inserted into a node type

    //check for the needed variables
    if (isset($data['name'])
        && isset($data['date'])
        && isset($data['email'])
    ) {
        $confirmation_message = drupal_mail("lfz", "info_session_confirmation", $data['email'] . ", info@learningfuze.com", user_preferred_language(null), array(
            "body" => theme("info_session_email_template", array("name" => $data['name'], "date" => $data['date'])),
            "subject" => "Info-Session RSVP Confirmed!"
        ));

        if (!isset($confirmation_message['result']) || !$confirmation_message['result']) {
            watchdog("lfz", "Error sending confirmation email", array(), WATCHDOG_ERROR);
        }
    } else {
        watchdog("lfz", "Data passed from the info session form seems to be incorrect. @data", array("@data" => $data), WATCHDOG_ERROR);
    }


    drupal_json_output($r);
}

function mentorship_program_callback()
{
    $r = "<p>Welcome to the mentorship landing page. The 6 weeks following the program is crucial for you to hone your skills
        based on what you want to do with your career. Most developers coming out of the program need guidance towards that
        path and we have put together some steps to help us help you as best as we can. Please fill out the questionaire before you schedule time with a mentor.</p>";
    $r .= '<ul class=col-sm-4><li class="list-group-item"><span class="glyphicon glyphicon-th-list">&nbsp;</span>' . l("Questionaire", "mentorship/questionaire") . '</li>';
    $r .= '<li class="list-group-item"><span class="glyphicon glyphicon-calendar">&nbsp;</span>' . l("Schedule Time", 'mentorship/booking') . '</li></ul>';

    return $r;
}

function mentorship_questionaire_callback()
{
    return '<iframe src="https://docs.google.com/a/learningfuze.com/forms/d/1KBW0wj3qFZmoEfIuYU_rM7lMh3lXwjqDlWnCkXQBBlQ/viewform" style="border:none; height:1000px;" class="col-sm-12"><iframe>';
}

function mentorship_booking_callback()
{
    return '<iframe src="https://lfzmentors.youcanbook.me/" style="border:none; height:1000px;" class="col-sm-12"><iframe>';
}

function ajax_website_form_handler_callback()
{
    _check_cross();

    $r = array(
        "success" => false
    );

    if (!empty($_POST)) {
        $data = $_POST;
        $title = (isset($data['formId'])) ? $data['formId'] : 'unknown-title';
        $success_msg = (isset($data['success_msg'])) ? $data['success_msg'] : "Thank you for submitting your info";
        $data['http_referer'] = (isset($_SERVER['HTTP_REFERER'])) ? $_SERVER['HTTP_REFERER'] : 'Data not validated';
        $encoded_data = json_encode($data);
        $form_data_capture = array(
            'type' => 'form_data_capture',
            'title' => $title,
            'body' => array(
                'und' => array(
                    array(
                        'value' => $encoded_data,
                        'summary' => NULL,
                        'format' => NULL,
                        'safe_value' => htmlentities($encoded_data),
                        'safe_summary' => NULL
                    )
                )
            ),
        );

        $new_node = _create_node($form_data_capture);

        if ($new_node) {
            $r['success'] = true;
            $r['data'] = array('success_msg' => $success_msg);

            $r['data']['redirect'] = website_form_redirect($data['formId']);

            $data['node'] = $new_node;
        } else {
            $r['error'] = 'unable to save data';
        }

    }else{
        $r['error'] = 'Data not avaialbe';
    }

    website_form_auto_responders($data, $r['success']);

    drupal_json_output($r);
    exit;
}

