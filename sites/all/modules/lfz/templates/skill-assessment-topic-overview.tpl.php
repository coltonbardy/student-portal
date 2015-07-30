<div class="col-xs-12">
    <h4>Topics with Low Test Scores</h4>

    <div>

        <!-- Nav tabs -->
        <ul class="nav nav-tabs" role="tablist">
            <li role="presentation" class="active">
                <a href="#question" aria-controls="home" role="tab" data-toggle="tab">By Questions</a>
            </li>
            <li role="presentation">
                <a href="#user" aria-controls="profile" role="tab" data-toggle="tab">By User</a>
            </li>
        </ul>

        <!-- Tab panes -->
        <div class="tab-content">
            <div role="tabpanel" class="tab-pane active" id="question">
                <div class="list-group">
                    <?php
                    $i = 0;

                    foreach ($data as $key => $sa_question):
                        $id = 'topic' . $i;
                        $user_count = count($sa_question['users']);
                        $label = 'default';
                        if ($user_count > 4) {
                            $label = 'danger';
                        } else if ($user_count > 1) {
                            $label = 'warning';
                        } else {
                            $label = 'success';
                        }
                        ?>
                        <div class="topic-list panel-group" id="accordion">
                            <div class="panel panel-<?php print $label;?>">
                                <div class="panel-heading">
                                    <a data-toggle="collapse" data-parent="#accordion"
                                       href="#<?php print $id; ?>"> <?php print $key; ?>
                                        <span class="label label-<?php print $label;?>"># of issues
                        : <?php print count($sa_question['users']);?></span></a>
                                </div>

                                <div class="questions panel-body panel-collapse collapse" id="<?php print $id; ?>">
                                    <?php foreach ($sa_question['questions'] as $q): ?>
                                        <div class="question-list">
                                            <h4><span
                                                    class="glyphicon glyphicon-question-sign"></span> <?php print ucfirst($q['question']); ?>
                                                - <?php print l('View Test', $q['sa_links']['student'], array('attributes' => array('target' => '_blank', 'class' => 'btn btn-info btn-xs'))); ?>
                                            </h4>
                                            <h5>Correct Answers -
                                                <small><?php print $q['correct_answer']; ?></small>
                                            </h5>
                                            <h5>User(s) Answers</h5>

                                            <div class="user-list well">
                                                <?php
                                                foreach ($q['user_answers'] as $user_answer): ?>
                                                    <h4><?php print $user_answer['user_info']['name']; ?> :
                                                        <small><?php print $user_answer['response']; ?></small>
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
            </div>
            <div role="tabpanel" class="tab-pane" id="user">...</div>
        </div>

    </div>
</div
