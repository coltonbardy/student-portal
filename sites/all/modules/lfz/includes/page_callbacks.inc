<?php


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

    if (_remove_agenda_item_from_agenda($agenda_node, $item_nid, $index)) {
        $res['data'] = $agenda_node;
    } else {
        $res['success'] = false;
        $res['data'] = 'Error saving node';
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

            //old agenda remove index to add it back in
            if (!_remove_agenda_item_from_agenda($remove_from_agenda, $item_nid, $current_index)) {
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

        //get user classes and then redirect if the user only has access to one class
        $users_classes = _get_users_classes();
        if (count($users_classes) === 1) {
            $redirect_url = str_replace("{class_nid}", $users_classes[0]->nid, $redirect_url);
            drupal_goto($redirect_url);
        }


        drupal_add_js(array('lfz' => array('redirect_url' => $redirect_url)),
            'setting');

        return _agenda_select_class('week');
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

    return theme('weekly_agenda', array('days' => $week_results));
}

function todays_agenda_callback($class_nid = false, $date = false)
{

    if (!$class_nid) {

        $redirect_url = 'class/{class_nid}/today';
        if ($date) {
            $redirect_url .= '/' . $date;
        }

        //get user classes and then redirect if the user only has access to one class
        $users_classes = _get_users_classes();
        if (count($users_classes) === 1) {
            $redirect_url = str_replace("{class_nid}", $users_classes[0]->nid, $redirect_url);
            drupal_goto($redirect_url);
        }

        drupal_add_js(array('lfz' => array('redirect_url' => $redirect_url)),
            'setting');
        return _agenda_select_class('today');
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
    $return = drupal_get_form('lfz_agenda_form', $todays_agenda->nid, $class->nid);

    //include js file
    drupal_add_js(drupal_get_path('module', 'lfz') . "/misc/todays-agenda.js");
    drupal_add_js(drupal_get_path('module', 'lfz') . "/misc/agenda.js");

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

function skill_assessment_overview_callback()
{
    $data = _get_assessment_data();

    $all_data = array();

    $issue_topic_data = array();

    foreach ($data as $d) {
        $sa_results = _get_results_data($d['nid']);

        foreach ($sa_results as $sa) {
            if(!isset($sa['user'])){
                continue;
            }
            $users = $sa['user'];

            foreach ($users as $user) {
                $topics = $user['data']['suggested_improvement']['topics'];

                foreach ($topics as $topic) {
                    if (!isset($issue_topic_data[$topic['name']])) {
                        $issue_topic_data[$topic['name']] = array(
                            'questions'=>array(),
                        );
                    }

                    $result_nid = $d['nid'];

                    $links = _get_sa_link_by_result_nid($result_nid);

                    $key = $result_nid . '-' . $topic['question'];

                    $issue_topic_data[$topic['name']]['questions'][$key] = array(
                        'results_nid'=>$result_nid,
                        'sa_links'=>$links,
                        'question' => $topic['question'],
                        'user_answers' => array(),
                        'correct_answer'=>$topic['answer']
                    );

                    $issue_topic_data[$topic['name']]['questions'][$key]['user_answers'][$user['uid']] = array(
                        "user_info"=> _get_user_get_basic_info($user['uid']),
                        "response"=>$topic['response']
                    );

                    if(!isset($issue_topic_data[$topic['name']]['users'])){
                        $issue_topic_data[$topic['name']]['users'] = array();
                    }
                    $issue_topic_data[$topic['name']]['users'][$user['uid']] = $user['uid'];
                }
            }

        }
    }

    function sort_topic_data($a, $b){

        if(count($a['users']) == count($b['users'])){
            return 0;
        }

        return (count($a['users']) < count($b['users']))? 1 : -1;
    }

    uasort($issue_topic_data, "sort_topic_data");

    $r = theme('skill_assessment_topic_overview', array('data'=>$issue_topic_data));

    return $r;
}