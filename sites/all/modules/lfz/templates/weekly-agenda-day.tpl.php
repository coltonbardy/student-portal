<?php
$panel_class = 'panel panel-default';
if (isset($is_today) && $is_today) {
    $panel_class .= ' panel-info';
}

if (!isset($attr['class'])) {
    $attr['class'] = '';
}

$attr['class'] .= ' list-group';

?>
<div class="<?php echo $panel_class; ?>">
    <div class="panel-heading"><?php echo $link; ?></div>
    <div class="panel-body">
        <div <?php echo drupal_attributes($attr); ?> >
            <?php
            if (count($items) > 0):
                ?>
                <?php
                foreach ($items as $item):
                    ?>
                    <?php echo theme('agenda_item_list', $item);?>
                <?php
                endforeach;
                ?>
            <?php endif; ?>
        </div>
    </div>
</div>