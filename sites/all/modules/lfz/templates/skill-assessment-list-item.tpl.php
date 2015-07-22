<?php
$node = node_load($item['nid']);
$class_nid = $node->field_class_reference['und'][0]['nid'];

//check if new, changed in the last 24 hours
$seconds_since_changed = time() - $node->changed;
$new = ($seconds_since_changed < 24 * 60 * 60) ? ' - <span class="label label-primary">Newly added</span>' : "";
$header = ucwords($item['title']).$new;
$calendar_badge = theme("calendar_badge", array("time"=>$node->changed));
$attributes = array("class" => "skill-assessment-list-item");

$content = l("View Results", "skillassessment/details/" . $item['nid']);

?>

<div class="list-group-item">
    <?php print theme("bootstrap_media_object", array("media_alignment"=>"media-left", "media_html"=>$calendar_badge, "heading"=>$header, "content"=>$content));?>
</div>