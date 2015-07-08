<?php

if (!isset($attr)) {
    $attr = array();
}

if (!isset($attr['class'])) {
    $attr['class'] = '';
}

$attr['class'] .= ' agenda-list-item';
$attr['id'] = 'nid-' . $nid;
$attr['data-nid'] = $nid;

$ob = ini_get('output_buffering');

if (!$ob) {
    ini_set('output_buffering', 'on');
}

ob_start();
?>
<?php echo theme('agenda_item_heading', array('title' => $title, 'nid' => $nid, 'type' => $type));

if (user_access('remove agenda items') && $removeBtn):
    ?>
    <input type="button" class="btn btn-sm btn-danger pull-right remove-agenda-item" value="Remove">
<?php
endif;
?>
    <span class="clearfix"></span>
<?php
$template_content = ob_get_contents();
ob_end_clean();

echo theme('bootstrap_list_item', array('content' => $template_content, 'attr' => $attr));
?>