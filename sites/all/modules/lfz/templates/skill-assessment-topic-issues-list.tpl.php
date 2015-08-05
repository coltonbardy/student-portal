<div class="col-xs-12">
    <h4>Incorrect Responses Grouped by Topic</h4>
    <div>

        <!-- Nav tabs -->
        <ul class="nav nav-tabs" role="tablist">
            <li role="presentation" class="active">
                <a href="#question" aria-controls="home" role="tab" data-toggle="tab">All Users</a>
            </li>
            <li role="presentation">
                <a href="#user" aria-controls="profile" role="tab" data-toggle="tab">By User</a>
            </li>
        </ul>

        <!-- Tab panes -->
        <div class="tab-content">
            <div role="tabpanel" class="tab-pane active" id="question">
                <?php print theme('skill_assessment_topic_issues_list_question', array("data"=>$data['topic_by_question'])); ?>
            </div>
            <div role="tabpanel" class="tab-pane" id="user">
                <?php print theme('skill_assessment_topic_issues_list_user', array("data"=>$data['topic_by_user'])); ?>
            </div>
        </div>

    </div>
</div>