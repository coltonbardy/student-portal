<?php

$node = node_load($nid);

if ($links) {
    $title = l($title, 'node/' . $nid);
    if (user_has_role(array_search('content manager', user_roles()))) {
        $edit = l('Edit', 'node/' . $nid . '/edit');
    }
}

?>

<div class="col-sm-9">
    <h5>
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