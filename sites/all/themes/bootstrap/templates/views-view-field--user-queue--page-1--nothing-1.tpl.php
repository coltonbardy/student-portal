<?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */


if (count($row->field_field_queue_item_status) > 0
    && isset($row->field_field_queue_item_status[0]['raw']['value'])
){
    $raw_value = $row->field_field_queue_item_status[0]['raw']['value'];
}

$nid = (isset($row->nid))?$row->nid:0;

$lfz_output = $output;

switch ($raw_value) {
    case 'waiting':
        $lfz_output = '<div class="col-xs-3">';
        $lfz_output .= l('<div class="btn btn-info pull-right">Resolve</div>', 'question-queue/resolve/'.$nid, array('html' => true));
        $lfz_output .= '</div>';
        break;
    case 'complete':
        $lfz_output = "";
        break;
    case 'open':
        $lfz_output = "";
        break;
    case 'reopened':
        $lfz_output = "";
        break;
}

?>

<?php print $lfz_output; ?>