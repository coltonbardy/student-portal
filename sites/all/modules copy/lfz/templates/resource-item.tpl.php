<?php
$type = $node->field_resource_type['und'][0]['value'];
$val = $node->field_instructor_reference_link['und'][0]['value'];
$class = "resource-link ";
$class .= (strstr($val, 'google')) ? 'resource-google' : '';
$instructor_link = (isset($node->field_instructor_reference_link['und']))?$node->field_instructor_reference_link['und'][0]['value']:false;
$student_link = (isset($node->field_reference_link['und']))?$node->field_reference_link['und'][0]['value']:false;

if (user_has_role(array_search('instructor', user_roles()))) {
    $link = $instructor_link;
}else if (user_has_role(array_search('student', user_roles()))) {
    $link = $student_link;
}

$link_content = '<span class="center-block"></span><p class="text-center">' . $node->title . ' - ' . $type . '</p>';
print l($link_content, $link, array('html' => true, 'attributes' => array('class' => $class)));

?>