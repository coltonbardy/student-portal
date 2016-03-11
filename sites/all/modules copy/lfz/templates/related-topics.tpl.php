<?php
if (count($topics) > 0):
    ?>
    <div class="panel panel-default">
        <div class="panel-heading">
            <h3 class="panel-title">Related Topics</small></h3>
        </div>
        <div class="panel-body">
            <div class="col-xs-12">
                <?php

                foreach ($topics as $topic) {
                    $node = $topic['node'];
                    print '<li>';
                    print l($node->title, 'node/' . $node->nid);
                    print '</li>';
                }
                ?>
            </div>
        </div>
    </div>

<?php
endif;
?>