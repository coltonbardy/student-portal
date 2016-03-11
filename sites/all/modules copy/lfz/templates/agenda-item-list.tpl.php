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

//check for resource type, otherwise use node type
$type = (isset($resource_type)) ? $resource_type : $node_type;

//skill assessment results
$results = _get_results_by_sa_nid($nid);

$ob = ini_get('output_buffering');

if (!$ob) {
    ini_set('output_buffering', 'on');
}

ob_start();
?>
<?php
print theme('agenda_item_heading', array('title' => $title, 'nid' => $nid, 'type' => $type));
?>
    <div class="btn-group col-xs-4 agenda-list-actions" role="group" aria-label="">
<?php if (user_access('remove agenda items') && $removeBtn): ?>
    <input type="button" class="btn btn-sm btn-danger remove-agenda-item" value="Remove">
<?php endif;

if (user_has_role(array_search('instructor', user_roles()))
    && $type == 'sa'
    && !$results
) {
    ?>
    <input type="button" class="btn btn-sm btn-info add-sa-results" value="Add Results">
<?php
}else if ($results && count($results) > 0) {
    //also needs to be changed inside of agenda.js
    $result_url = "skillassessment/details/" . $results['nid'];
    print l('View Resutls', $result_url, array('attributes' => array('class' => 'btn btn-sm btn-default', 'target' => '_blank')));
}

?>
    </div>
    <span class="clearfix"></span>
<?php
$template_content = ob_get_contents();
ob_end_clean();

print theme('bootstrap_list_item', array('content' => $template_content, 'attr' => $attr));
?>