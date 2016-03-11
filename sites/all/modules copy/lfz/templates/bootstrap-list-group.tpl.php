<?php
if (!isset($attr)) {
	$attr = array();
}
if (!isset($attr['class'])) {
	$attr['class'] = '';
}
$attr['class'] .= ' list-group';

?>
<div <?php echo drupal_attributes($attr);?> >
<?php
if (isset($content)) {
	echo $content;
} else if (isset($items)) {
	foreach ($items as $item) {
		echo theme('bootstrap_list_item', $item);
	}
} else {
	?>
	<h4>No Content for list group to display</h4>
	<?php
}
?>
</div>