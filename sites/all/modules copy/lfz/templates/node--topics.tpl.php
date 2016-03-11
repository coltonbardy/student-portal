<?php

$resources = $field_related_resources;
$skills = $field_related_skills;

$student_comments = array_merge(array(), $content['comments']);
$instructor_comments = array_merge(array(), $content['comments']);
$student_comments['#lfz'] = array('show_role' => 'student', 'show_comment_form' => true, 'show_user_thumb' => true);
$instructor_comments['#lfz'] = array('show_role' => 'instructor');

//echo '<pre>';
//print_r(array_keys(get_defined_vars()));
//echo '</pre>';
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> row clearfix"<?php print $attributes; ?>>
    <div class="col-xs-8">
        <div class="row">
            <div class="col-xs-12">
                <div class="panel panel-default">
                    <div class="panel-body">
                        <?php print render($content['body']); ?>
                    </div>
                </div>
            </div>
            <div class="col-xs-12">
                <?php print theme('related_topics', array('topics'=>$field_related_topics)); ?>
            </div>
            <div class="col-xs-12">
                <?php print theme('resource_list', array('resources'=>$resources)); ?>
            </div>
            <div class="col-xs-12">
                <?php print render($student_comments); ?>
            </div>
        </div>
    </div>
    <div class="col-xs-4">
        <div class="row">
            <div class="col-xs-12">
                <?php print theme('skills_list', array('skills'=>$skills)); ?>
            </div>
            <div class="col-xs-12">
                <?php print theme('instructor_comments', array('comments'=>$instructor_comments)); ?>
            </div>
        </div>
    </div>
</div>
