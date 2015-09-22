<?php

$node = node_load($nid);

if ($links) {
    if(user_access('view agenda item details')){
        $title = l($title, 'node/' . $nid);
    }
    if (user_has_role(array_search('content manager', user_roles()))) {
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

?>

<div class="col-sm-8">
    <h5>
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