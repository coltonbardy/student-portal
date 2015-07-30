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
                <?php print theme('skill_assessment_topic_issues_list'); ?>
            </div>
            <div role="tabpanel" class="tab-pane" id="user">...</div>
        </div>

    </div>
</div>