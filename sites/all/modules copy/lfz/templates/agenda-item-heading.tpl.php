<?php

$node = node_load($nid);


if ($links) {
    if(user_access('view agenda item details')){
        $title = l($title, 'node/' . $nid);
    }
    if (user_access('edit agenda item details')) {
        $edit = l('Edit', 'node/' . $nid . '/edit');
    }

    $icon = "";

    if($type === 'event'){
        $icon = '<span class="glyphicon glyphicon-calendar glyphicon-align-left" aria-hidden="true">';

        $direct_link = (count($node->field_reference_link) > 0)?$node->field_reference_link['und'][0]['value']:false;
        if($direct_link){
            $title = l($node->title, $direct_link, array("attributes"=>array("target"=>"_blank")));
        }
    }
}

//give default value for a label
$label = "";

$resource_links = array();

//get all resource links and put them into an array
if(isset($node->field_reference_link)){
    $resource_links = $node->field_reference_link;
    //if user has instructor role then add instructor links
    if (user_has_role(array_search('instructor', user_roles()))) {
        $resource_links += $node->field_instructor_reference_link;
    }
}

//if there are no resource links, (which happens when resources aren't built yet) we add a label to the agenda item
if(count($resource_links) == 0){
    $label = "<small class=\"label label-warning\">Coming Soon</small>";
}

?>

<div class="col-sm-8">
    <h5>
        <?php echo $label; ?>
        <?php echo $icon ; ?>
        <?php echo $title; ?>
        <?php if ($type): ?>
            -
            <small><?php echo $type; ?></small>
        <?php endif; ?>
        <?php if (isset($edit)): ?>
            -
            <small><?php echo $edit; ?></small>
        <?php endif; ?>
    </h5>
</div>