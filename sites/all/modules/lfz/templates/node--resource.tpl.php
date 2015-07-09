<?php
$links = $field_reference_link;
//if user has instructor role then add instructor links
if (user_has_role(array_search('instructor', user_roles()))) {
    $links += $field_instructor_reference_link;
}

$skills = $field_related_skills;

$student_comments = array_merge(array(), $content['comments']);
$instructor_comments = array_merge(array(), $content['comments']);
$student_comments['#lfz'] = array('show_role' => 'student', 'show_comment_form' => true, 'show_user_thumb' => true);
$instructor_comments['#lfz'] = array('show_role' => 'instructor');
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> row clearfix"<?php print $attributes; ?>>
    <div class="col-xs-8">
        <div class="row">
            <div class="col-xs-12">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title">Resource Links</small></h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <?php
                            foreach ($links as $key => $link):
                                $val = $link['value'];
                                $class = "resource-link resource-type-" . $field_resource_type[0]['value'] . ' ';
                                $class .= (strstr($val, 'google')) ? 'resource-google' : ''
                                ?>
                                <div class="col-xs-4 resource-link-con">
                                    <a class="<?php print $class;?>" href="<?php print $val;?>" target="_blank">
                                        <span class="center-block"></span>

                                        <p class="text-center"><?php print $title;?></p></a>
                                </div>
                            <?php
                            endforeach;
                            ?>
                        </div>
                    </div>

                </div>
            </div>
            <div class="col-xs-12">
                <?php
                print render($student_comments); ?>
            </div>
        </div>
    </div>
    <div class="col-xs-4">
        <div class="row">
            <div class="col-xs-12">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title">Skills</small></h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <?php
                            foreach ($skills as $key => $skill):
                                $skill_node = $skill['node'];
                                $title = $skill_node->title;
                                $icon_uri = (isset($skill_node->field_icon['und'])) ? $skill_node->field_icon['und'][0]['uri'] : false;
                                $url = 'skills/' . $skill_node->nid;
                                if ($icon_uri) {
                                    $icon_url = file_create_url($icon_uri);
                                    $image_theme_array = array(
                                        "path" => $icon_url,
                                        "attributes" => array(
                                            "data-toggle" => "tooltip",
                                            "data-placement" => "top",
                                            "title" => $title,
                                            "width" => 25,
                                        ),
                                    );
                                    $skill_output = l(theme_image($image_theme_array), $url, array("html" => true));
                                } else {
                                    $skill_cat_nid = $skill_node->field_skill_category['und'][0]['nid'];
                                    $skill_output = l($title, $url, array('attributes' => array('class' => 'skill-link')));
                                }

                                ?>
                                <div class="col-xs-4 skill-link-con">
                                    <?php print $skill_output;?>
                                </div>
                            <?php
                            endforeach;
                            ?>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-xs-12">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title">Instructor Comments</h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <?php
                            print render($instructor_comments); ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
