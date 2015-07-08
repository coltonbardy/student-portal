<div class="panel panel-default">
    <div class="panel-heading">
        <h3 class="panel-title">Related Resources</small></h3>
    </div>
    <div class="panel-body">
        <div class="row">
            <?php
            foreach ($resources as $key => $resource):
                ?>
                <div class="col-xs-4 resource-link-con">
                    <?php print theme('resource_item', array('node'=>$resource['node'])); ?>
                </div>
            <?php
            endforeach;
            ?>
        </div>
    </div>

</div>