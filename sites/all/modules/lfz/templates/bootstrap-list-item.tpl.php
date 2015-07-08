<?php
if (!isset($attr)) {
	$attr = array();
}
if (!isset($attr['class'])) {
	$attr['class'] = '';
}
$attr['class'] .= ' list-group-item';

?>

<div <?php echo drupal_attributes($attr);?> >
<?php echo $content;?>
</div>