<div class="panel-group" id="accordion" role="tablist" aria-multiselectable="true">

    <?php if (count($data) === 0): ?>
        <h3>No Data Available</h3>
    <?php
    endif;

    $i = 0;
    foreach ($data as $user_data):
        
        $user = user_load($user_data['uid']);
        $user_name = $user->field_first_name['und'][0]['value'] . ' ' . $user->field_last_name['und'][0]['value'];
        $element_id = "collapse" . $user->uid;
        $details_class = "panel-collapse collapse";
        if ($i === 0 && count($data) == 1) {
            $details_class .= " in";
        }
        $percent = $user_data['data']['total_correct'] / $user_data['data']['total_possible'];

        $class_to_grade = array(
            'label-success' => .9,
            'label-info' => .7,
            'label-warning' => .5,
            'label-danger' => 0
        );

        $label_class = "label";
        //unknown answered vs total number of questions to get unknown percent
        $not_answered_percent = (($user_data['data']['unknown_answered'] / count($user_data['questions']))*100);

        foreach ($class_to_grade as $key => $value) {
            if ($percent >= $value) {
                $label_class .= " " . $key;
                break;
            }
        }

        ?>
        <div class="panel panel-default">
            <div class="panel-heading" role="tab" id="headingOne">
                <h4 class="panel-title">
                    <a role="button" data-toggle="collapse" data-parent="#accordion" href="#<?php print $element_id; ?>"
                       aria-expanded="true" aria-controls="collapseOne">
                        <?php print $user_name; ?>
                        <span
                            class="<?php print $label_class; ?>"><?php print round(($percent*100)).'% answered correctly' ?></span>
                        <span
                            class="label label-default"><?php print round($not_answered_percent).'% not answered' ?></span>
                    </a>
                </h4>
            </div>
            <div id="<?php print $element_id; ?>" class="<?php print $details_class; ?>" role="tabpanel"
                 aria-labelledby="headingOne">
                <div class="panel-body">
                    <div class="col-xs-6">
                        <h4>Questions & Answers</h4>
                        <?php
                        foreach ($user_data['questions'] as $question):
                            $panel_class = ($question['correct']) ? 'panel panel-success' : 'panel panel-danger';
                            ?>
                            <div class="<?php print $panel_class; ?>">
                                <div class="panel-heading">
                                    <?php print htmlspecialchars($question['question']); ?>
                                </div>
                                <div class="panel-body">
                                    <p>User Reponse : <?php print htmlspecialchars($question['response']); ?></p>
                                </div>
                                <?php if (!$question['correct']): ?>
                                    <div class="panel-footer">Correct Answer : <?php print htmlspecialchars($question['answer']); ?></div>
                                <?php endif; ?>

                            </div>

                        <?php
                        endforeach;
                        ?>
                    </div>
                    <div class="col-xs-6">
                        <h4>Suggested Items To Review</h4>
                        <div class="well well-sm">
                            <ul class="list-group">
                                <?php foreach ($user_data['data']['suggested_improvement']['topics'] as $topic): ?>
                                    <li class="list-group-item"><?php print $topic; ?></li>
                                <?php endforeach; ?>
                            </ul>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <?php
        $i++;
    endforeach;
    ?>
</div>