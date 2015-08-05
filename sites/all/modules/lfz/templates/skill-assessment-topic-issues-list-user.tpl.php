<div class="list-group">
    <?php


    foreach ($data as $user_topic_data):
        $id = 'user-' . $user_topic_data['user_info']['uid'];

        //any user without info should be skipped
        if (trim($user_topic_data['user_info']['name']) == "") {
            continue;
        }
        ?>

        <div class="topic-list panel-group" id="user-accordion">
            <div class="panel panel-default">
                <div class="panel-heading">
                    <a data-toggle="collapse" data-parent="#user-accordion"
                       href="#<?php print $id; ?>"> <?php print $user_topic_data['user_info']['name']; ?>
                        <span class="label label-default">Topics Covered
                                : <?php print count($user_topic_data['topics']);?></span></a>
                </div>

                <div class="panel-body panel-collapse collapse" id="<?php print $id; ?>">
                    <div class="list-group">
                        <?php
                        foreach ($user_topic_data['topics'] as $topic):
                            $label = 'default';
                            if ($topic['percent']  < 50) {
                                $label = 'danger';
                            } else if ($topic['percent'] < 80) {
                                $label = 'warning';
                            } else {
                                $label = 'success';
                            }

                            ?>
                            <div class="list-group-item list-group-item-<?php print $label;?>">
                                <?php print $topic['title']; ?>
                                <span class="label label-<?php print $label; ?>"><?php print $topic['percent']. '% - '.$topic['total_correct']." out of ".$topic['total_questions'] ;?></span>
                            </div>
                        <?php endforeach; ?>
                    </div>
                </div>
            </div>
        </div>
    <?php
    endforeach;
    ?>

</div>