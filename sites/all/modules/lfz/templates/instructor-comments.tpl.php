<?php
if (count($comments['comments']) > 0):
    ?>
    <div class="panel panel-default">
        <div class="panel-heading">
            <h3 class="panel-title">Instructor Comments</h3>
        </div>
        <div class="panel-body">
            <div class="row">
                <?php print render($comments); ?>
            </div>
        </div>
    </div>

<?php
endif;
?>