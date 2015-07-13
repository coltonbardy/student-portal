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
        if ($i === 0) {
            $details_class .= " in";
        }
        $percent = $user_data['data']['total_correct'] / $user_data['data']['total_possible'];

        $class_to_grade = array(
            'label-success' => .9,
            'label-default' => .7,
            'label-warning' => .6,
            'label-danger' => 0
        );

        $label_class = "label";

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
                            class="<?php print $label_class; ?>"><?php print $user_data['data']['total_correct'] . " of " . $user_data['data']['total_possible'] ?></span>
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
                                    <?php print $question['question']; ?>
                                </div>
                                <div class="panel-body">
                                    <p>User Reponse : <?php print $question['response']; ?></p>
                                </div>
                                <?php if (!$question['correct']): ?>
                                    <div class="panel-footer">Correct Answer : <?php print $question['answer']; ?></div>
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
                                <?php foreach ($user_data['data']['suggested_improvement'] as $improvement): ?>
                                    <li class="list-group-item"><?php print $improvement['topic']; ?></li>
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