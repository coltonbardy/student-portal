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