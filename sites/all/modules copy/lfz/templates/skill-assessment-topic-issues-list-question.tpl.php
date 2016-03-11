<div class="list-group">
    <?php
    $i = 0;

    foreach ($data as $topic => $topic_data):
        $id = 'topic' . $i;

        $label_class = 'default';
        if ($topic_data['percent'] < 50) {
            $label_class = 'danger';
        } else if ($topic_data['percent']  < 80) {
            $label_class = 'warning';
        } else {
            $label_class = 'success';
        }
        $topic_label = $topic_data['percent']."% ".$topic_data['total_correct'].' out of '.$topic_data['total_questions'];
        ?>
        <div class="topic-list panel-group" id="topic-accordion">
            <div class="panel panel-<?php print $label_class;?>">
                <div class="panel-heading">
                    <a data-toggle="collapse" data-parent="#topic-accordion"
                       href="#<?php print $id; ?>"> <?php print $topic; ?>
                        <span class="label label-<?php print $label_class;?>"><?php print $topic_label;?></span></a>
                </div>

                <div class="topic-questions panel-body panel-collapse collapse" id="<?php print $id; ?>">
                    <?php foreach ($topic_data['questions'] as $q): ?>
                        <div class="question-list">
                            <h4><span
                                    class="glyphicon glyphicon-question-sign"></span> <?php print ucfirst($q['question']); ?>
                                - <?php print l('View Test', $q['sa_link']['student'], array('attributes' => array('target' => '_blank', 'class' => 'btn btn-info btn-xs'))); ?>
                            </h4>
                            <h5>Correct Answers -
                                <small><?php print $q['answer']; ?></small>
                            </h5>
                            <h5>Incorrect User(s) Answers</h5>

                            <div class="user-list well">
                                <?php
                                foreach ($q['user_responses'] as $user_response): ?>
                                    <h4><?php print $user_response['user_info']['name']; ?> :
                                        <small><?php print $user_response['response']; ?></small>
                                    </h4>
                                <?php endforeach; ?>
                            </div>
                        </div>
                    <?php
                    endforeach;
                    ?>
                </div>
            </div>
        </div>

        <?php
        $i++;
    endforeach;
    ?>
</div>