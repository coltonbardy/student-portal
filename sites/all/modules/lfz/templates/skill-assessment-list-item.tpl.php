<?php
    $node = node_load($item['nid']);
    $class_nid = $node->field_class_reference['und'][0]['nid'];
?>
<div class="skill-assessment-list-item">
    <h3><?php print l($item['title'], "skillassessment/details/".$item['nid']); ?></h3>
</div>