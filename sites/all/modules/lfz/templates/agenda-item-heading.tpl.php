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
    <div class="panel">
        <div class="panel-body">
            Anim pariatur cliche reprehenderit, enim eiusmod high life accusamus terry richardson ad squid. 3 wolf moon
            officia aute, non cupidatat skateboard dolor brunch. Food truck quinoa nesciunt laborum eiusmod. Brunch 3 wolf
            moon tempor, sunt aliqua put a bird on it squid single-origin coffee nulla assumenda shoreditch et. Nihil anim
            keffiyeh helvetica, craft beer labore wes anderson cred nesciunt sapiente ea proident. Ad vegan excepteur
            butcher vice lomo. Leggings occaecat craft beer farm-to-table, raw denim aesthetic synth nesciunt you probably
            haven't heard of them accusamus labore sustainable VHS.
        </div>
    </div>
</div>