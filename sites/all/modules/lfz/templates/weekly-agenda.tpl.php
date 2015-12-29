<div class="col-xs-12">
    <div id="navigation" class=""><?php echo $last_week_link; ?> - <?php echo $next_week_link; ?></div>
</div>
<?php
foreach ($days as $d):
    ?>
    <div class="col-xs-12">
        <?php
        echo theme('weekly_agenda_day', $d);
        ?>
    </div>
<?php
endforeach;
?>