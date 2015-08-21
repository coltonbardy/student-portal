<!doctype html>
<!-- paulirish.com/2008/conditional-stylesheets-vs-css-hacks-answer-neither/ -->
<!--[if lt IE 7]>
<html class="no-js ie6 oldie" lang="en"> <![endif]-->
<!--[if IE 7]>
<html class="no-js ie7 oldie" lang="en"> <![endif]-->
<!--[if IE 8]>
<html class="no-js ie8 oldie" lang="en"> <![endif]-->
<!--[if gt IE 8]><!-->
<html class="no-js" lang="english"> <!--<![endif]-->

<head>
    <meta charset="utf-8">
    <title>Application | LearningFuze</title>
    <meta name="description" content="LZ"/>
    <meta name="keywords" content="LZ"/>

    <link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css" rel="stylesheet" type="text/css" />

    <link href="https://student.learningfuze.com/css/lz.css" rel="stylesheet" type="text/css" />

    <link href="https://student.learningfuze.com/css/jquery.fancybox.css" rel="stylesheet" type="text/css" media="screen" />

    <!-- JS -->
    <script src="https://code.jquery.com/jquery-1.11.0.min.js" type="text/javascript"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js" type="text/javascript"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.3/jquery.easing.min.js" type="text/javascript"></script>
    <script src="/js/jquery.fancybox.pack.js" type="text/javascript"></script>
    <script src="https://ajax.aspnetcdn.com/ajax/jquery.validate/1.12.0/jquery.validate.min.js" type="text/javascript"></script>
    <script src="https://ajax.aspnetcdn.com/ajax/jquery.validate/1.12.0/additional-methods.min.js" type="text/javascript"></script>
    <script src="/js/vendor/jquery.ui.widget.js" type="text/javascript"></script>
    <script src="/js/jquery.iframe-transport.js" type="text/javascript"></script>
    <script src="/js/jquery.fileupload.js" type="text/javascript"></script>
    <script>
        (function (i, s, o, g, r, a, m) {
            i['GoogleAnalyticsObject'] = r;
            i[r] = i[r] || function () {
                (i[r].q = i[r].q || []).push(arguments)
            }, i[r].l = 1 * new Date();
            a = s.createElement(o),
                m = s.getElementsByTagName(o)[0];
            a.async = 1;
            a.src = g;
            m.parentNode.insertBefore(a, m)
        })(window, document, 'script', '//www.google-analytics.com/analytics.js', 'ga');

        ga('create', 'UA-20114248-2', 'learningfuze.com');
        ga('send', 'pageview');

    </script>
</head>
<body>

<!-- Navigation -->
<nav class="navbar navbar-default navbar-fixed-top">
    <div class="navbar-inner">
        <div class="container">
            <!-- Brand and toggle get grouped for better mobile display -->
            <div class="navbar-header page-scroll">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
                    <span class="sr-only">Toggle navigation</span> <span class="icon-bar"></span>
                    <span class="icon-bar"></span> <span class="icon-bar"></span>
                </button>
                <a class="brand" href="http://learningfuze.com"><img src="/img/lz_logo.png" alt="" title="" height="25" border="0" width="251"/></a>
            </div>

            <!-- Collect the nav links, forms, and other content for toggling -->
            <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                <ul class="nav navbar-nav navbar-right">
                    <li class="hidden">
                        <a href="/"></a>
                    </li>
                    <li class="page-scroll">
                        <a href="/myhome"><span class="glyphicon glyphicon-home"></span> My Home</a>
                    </li>
                    <li>
                        <a href="/profile"><span class="glyphicon glyphicon-user"></span> Profile</a>
                    </li>
                    <li class="page-scroll">
                        <a href="/logout"><span class="glyphicon glyphicon-off"></span> Logout</a>
                    </li>
                </ul>
            </div>
            <!-- /.navbar-collapse -->
        </div>
    </div>
    <!-- /.container-fluid -->
</nav>

<div style="margin-top: 80px">
    <div class="container">
        <div class="row">
            <div id="content">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">Current Application Status</h3>
                    </div>
                    <button type="button" class="btn btn-question" data-fancybox-href="#question-container" id="btn-question">Ask a Question</button>
                    <div class="panel-body">
                        <strong>Incomplete</strong> <br/>
                        <span class="last-updated">Last Updated: 2015-08-06 5:16PM</span>
                    </div>
                </div>

                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title">Next Step:</h3>
                    </div>
                    <div class="panel-body">
                        Please complete and submit your application below.
                    </div>
                </div>
                <style>
                    #increment-bar-list {
                        list-style: none;
                        margin: 0 auto;
                        padding: 0;
                        text-align: center;
                    }

                    #increment-bar-list li {
                        width: 247px;
                        height: 38px;
                        background: url('/img/increment_bars.png') top left no-repeat;
                        background-position: 0 -38px;
                        display: inline-block;
                        margin-left: -20px;
                        position: relative;
                    }

                    #increment-bar-list li.inc-inactive {
                        background-position: 0 -76px;
                    }

                    #increment-bar-list li.inc-active {
                        background-position: 0 -38px;
                    }

                    #increment-bar-list li.inc-completed {
                        background-position: 0 0px;
                    }

                    #increment-bar-list li span {
                        display: block;
                        width: 234px;
                        height: 100%;
                        text-align: center;
                        line-height: 38px;
                        vertical-align: middle;
                        color: #fff;
                        font-size: 20px;
                        font-family: "Futura Md BT";
                    }

                    @font-face {
                        font-family: "Futura Md BT";
                        src: url("/fonts/FUTUMD_.woff");
                    }

                    .increment-container {
                        background: #b7b7b7;
                        padding: 10px;
                    }

                    #form-container {
                        width: 700px;
                        min-height: 750px;
                        position: relative;
                        margin: 0 auto;
                        overflow: hidden;
                    }

                    .questions-overflow {
                        width: 100%;
                        height: 480px;
                        overflow: scroll;
                        padding: 0 10px 10px 0;
                    }

                    .application-form {
                        position: absolute;
                        top: 0;
                        left: 1000px;
                        width: 100%;
                        height: 100%;
                    }

                    .application-form--on {
                        left: 0px;
                    }

                    .application-form--off {
                        left: 1000px;
                    }

                    .inc-button-next {
                        float: right;
                    }

                    .form-control.error {
                        border: 2px solid #dc1d00;
                    }

                    label.error, div.file-error {
                        color: #dc1d00;
                    }

                    .upload-header {
                        width: 100%;
                        border-bottom: 1px solid #000;
                        margin: 15px 0 5px 0;
                        font-size: 15px;
                        font-weight: bold;
                    }

                    #form-submit-application {
                        text-align: center;
                    }

                    h3 {
                        text-align: center;
                    }

                    .inc-button-prev {
                        float: left;
                    }
                </style>

                <div class="row increment-container">
                    <h3 id="flavor-text-container">Let's get to know you a little bit better!</h3>

                    <ul id="increment-bar-list">
                        <li style="z-index:10;" class="inc-active">
                            <span>Basic Information</span></li>
                        <li style="z-index:9;" class="inc-inactive">
                            <span>Questionnaire</span></li>
                        <li style="z-index:8;" class="inc-inactive">
                            <span>Submit Application</span></li>

                    </ul>
                </div>

                <div class="row" id="form-container">
                    <div class="form-container application-form application-form--on"id="form-basic">
                        <h3>1. Basic Information</h3>
                        <p>Complete the information below. You may update or change your information at any time by clicking the profile icon in the top right menu.</p>
                        <form role="form" class="form-horizontal" method="post">
                            <div class="form-group has-feedback">
                                <label for="fname" class="col-sm-2 control-label">First Name*</label>

                                <div class="col-sm-10">
                                    <input type="text" name="first_name" class="form-control" id="fname"
                                           value="Eric test"
                                           required/>
                                </div>
                            </div>

                            <div class="form-group has-feedback">
                                <label for="lname" class="col-sm-2 control-label">Last Name*</label>

                                <div class="col-sm-10">
                                    <input type="text" name="last_name" class="form-control" id="lname"
                                           value="johnson test"
                                           required/>
                                </div>
                            </div>

                            <div class="form-group has-feedback">
                                <label for="email" class="col-sm-2 control-label">Email*</label>

                                <div class="col-sm-10">
                                    <input type="tel" name="uacc_email" class="form-control" id="email"
                                           value="eric.johnson@learningfuze.com"
                                           required/>
                                </div>
                            </div>

                            <div class="form-group has-feedback">
                                <label for="phone" class="col-sm-2 control-label">Phone*</label>

                                <div class="col-sm-10">
                                    <input type="tel" name="phone" class="form-control" id="phone" value=""
                                           required/>
                                </div>
                            </div>
                            <div class="form-group has-feedback">
                                <label for="address1" class="col-sm-2 control-label">Address*</label>

                                <div class="col-sm-10">
                                    <input type="text" name="address1" class="form-control" id="address1"
                                           value="" required/>
                                </div>
                            </div>
                            <div class="form-group has-feedback">
                                <label for="address2" class="col-sm-2 control-label">Address2 (optional)</label>

                                <div class="col-sm-10">
                                    <input type="text" name="address2" class="form-control" id="address2"
                                           value=""/>
                                </div>
                            </div>
                            <br style="clear:both;"/>

                            <div class="form-group has-feedback">
                                <label for="city" class="col-sm-2 control-label">City*</label>

                                <div class="col-sm-10">
                                    <input name="city" type="text" class="form-control" id="city" value=""
                                           required/>
                                </div>
                            </div>
                            <div class="form-group has-feedback">
                                <label for="state" class="col-sm-2 control-label">State*</label>
                                <div class="col-sm-10">
                                    <select name="state" class="form-control">
                                        <option value="AL" >Alabama</option>
                                        <option value="AK" >Alaska</option>
                                        <option value="AZ" >Arizona</option>
                                        <option value="AR" >Arkansas</option>
                                        <option value="CA" >California</option>
                                        <option value="CO" >Colorado</option>
                                        <option value="CT" >Connecticut</option>
                                        <option value="DE" >Delaware</option>
                                        <option value="DC" >District Of Columbia</option>
                                        <option value="FL" >Florida</option>
                                        <option value="GA" >Georgia</option>
                                        <option value="HI" >Hawaii</option>
                                        <option value="ID" >Idaho</option>
                                        <option value="IL" >Illinois</option>
                                        <option value="IN" >Indiana</option>
                                        <option value="IA" >Iowa</option>
                                        <option value="KS" >Kansas</option>
                                        <option value="KY" >Kentucky</option>
                                        <option value="LA" >Louisiana</option>
                                        <option value="ME" >Maine</option>
                                        <option value="MD" >Maryland</option>
                                        <option value="MA" >Massachusetts</option>
                                        <option value="MI" >Michigan</option>
                                        <option value="MN" >Minnesota</option>
                                        <option value="MS" >Mississippi</option>
                                        <option value="MO" >Missouri</option>
                                        <option value="MT" >Montana</option>
                                        <option value="NE" >Nebraska</option>
                                        <option value="NV" >Nevada</option>
                                        <option value="NH" >New Hampshire</option>
                                        <option value="NJ" >New Jersey</option>
                                        <option value="NM" >New Mexico</option>
                                        <option value="NY" >New York</option>
                                        <option value="NC" >North Carolina</option>
                                        <option value="ND" >North Dakota</option>
                                        <option value="OH" >Ohio</option>
                                        <option value="OK" >Oklahoma</option>
                                        <option value="OR" >Oregon</option>
                                        <option value="PA" >Pennsylvania</option>
                                        <option value="RI" >Rhode Island</option>
                                        <option value="SC" >South Carolina</option>
                                        <option value="SD" >South Dakota</option>
                                        <option value="TN" >Tennessee</option>
                                        <option value="TX" >Texas</option>
                                        <option value="UT" >Utah</option>
                                        <option value="VT" >Vermont</option>
                                        <option value="VA" >Virginia</option>
                                        <option value="WA" >Washington</option>
                                        <option value="WV" >West Virginia</option>
                                        <option value="WI" >Wisconsin</option>
                                        <option value="WY" >Wyoming</option>
                                    </select>
                                </div>
                            </div>
                            <div class="form-group has-feedback">
                                <label for="zip" class="col-sm-2 control-label">Zip*</label>

                                <div class="col-sm-10">
                                    <input type="text" name="zip" class="form-control" id="zip" value=""
                                           required/>
                                </div>
                            </div>
                            <div class="form-group has-feedback">
                                <label for="dob" class="col-sm-4 control-label">DOB* (must be 18+)</label>

                                <div class="col-sm-8">
                                    <input type="text" name="dob" class="form-control" id="dob" placeholder="MM/DD/YYYY"
                                           value="" required/>
                                </div>
                            </div>
                            <button type="submit" class="btn btn-primary inc-button-next">Save and Continue</button>
                        </form>
                    </div>

                    <div
                        class="form-container application-form "
                        id="form-questions">
                        <h3>2. Questionnaire</h3>

                        <p>
                            These questions are pretty basic and will allow us to know more about your background, current state, and where
                            you want to go. Don't overthink these. Just complete them in earnest!
                        </p>

                        <p>All fields are required.</p>

                        <div class="questions-overflow">
                            <form role="form" method="post">
                                <div class="form-group has-feedback">
                                    <label>You are currently</label>
                                    <div class="radio">
                                        <label>
                                            <input type="radio" name="employment" value="0"
                                                   required checked>
                                            Employed full-time                        </label>
                                    </div>
                                    <div class="radio">
                                        <label>
                                            <input type="radio" name="employment" value="1"
                                                   required >
                                            Employed part-time and/or Student                        </label>
                                    </div>
                                    <div class="radio">
                                        <label>
                                            <input type="radio" name="employment" value="2"
                                                   required >
                                            Self-employed                        </label>
                                    </div>
                                    <div class="radio">
                                        <label>
                                            <input type="radio" name="employment" value="3"
                                                   required >
                                            Unemployed                        </label>
                                    </div>
                                </div>

                                <div class="form-group has-feedback">
                                    <label for="hypothetical">
                                        In a hypothetical conversation with someone that knows you very well, complete the following sentence about how they would say: "Oh he/she is GREAT, but be careful. He/she is a little too... (what?)." Explain.                                </label>
                                <textarea name="hypothetical" class="form-control" id="hypothetical"
                                          required></textarea>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="ideal-env">
                                        Describe your ideal work environment in 4 points or less                                </label>
                                <textarea name="ideal-env" class="form-control" id="ideal-env"
                                          required></textarea>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="things-you-are-not">
                                        List 5 things that you are NOT:                                </label>
                                <textarea name="things-you-are-not" class="form-control" id="things-you-are-not"
                                          required></textarea>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="weaknesses">
                                        What are your top 4 WEAKNESSES                                </label>
                                <textarea name="weaknesses" class="form-control" id="weaknesses"
                                          required></textarea>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="alternatives">
                                        What appeals to you about this program as opposed to other alternatives?                                </label>
                                <textarea name="alternatives" class="form-control" id="alternatives"
                                          required></textarea>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="todo-after">
                                        What do you want to do after the program that you can't do now?                                </label>
                                <textarea name="todo-after" class="form-control" id="todo-after"
                                          required></textarea>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="how-find">
                                        How did you hear about LearningFuze?                                </label>
                                <textarea name="how-find" class="form-control" id="how-find"
                                          required></textarea>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="class-start">Which start date are you interested in?</label>
                                    <select name="class-start" class="form-control" id="class-start">

                                        <option value="06/22/2015 - 09/11/2015" >06/22/2015 - 09/11/2015</option>

                                        <option value="09/21/2015 - 12/11/2015" >09/21/2015 - 12/11/2015</option>
                                    </select>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="prep-code">
                                        Describe what specific steps you have taken to learn coding on your own?                                </label>
                                <textarea name="prep-code" class="form-control" id="prep-code"
                                          required></textarea>
                                </div>
                                <div class="form-group has-feedback">
                                    <label for="prep-tech">
                                        What technologies or languages have you taken the initial steps to learn?                                </label>
                                <textarea name="prep-tech" class="form-control" id="prep-tech"
                                          required></textarea>
                                </div>

                                <button onclick="return go_to_form(0)" class="btn btn-primary inc-button-prev">Back</button>
                                <button type="submit" class="btn btn-primary inc-button-next">Save and Continue</button>
                            </form>
                        </div>
                    </div>

                    <div
                        class="form-container application-form "
                        id="form-resume">
                        <h3>3. Upload Resume</h3>

                        <p>Upload your current resume LearningFuze Style: include the stuff you don't like, don't want to show
                            employers, or don't think is relevant because you never know how we can use it as a strength!</p>

                        <p>In addition to the required resume, you may upload an optional cover letter.</p>

                        <p>Accepted file formats: pdf, doc, docx, txt</p>

                        <div class="upload-header">Resume*</div>
                        <input id="resume_upload" type="file" name="resume" data-url="/ajax/myhome/resume">

                        <div id="resume-status"></div>

                        <div class="upload-header">Cover Letter</div>
                        <input id="cover_upload" type="file" name="cover" data-url="/ajax/myhome/resume">

                        <div id="cover-status"></div>

                        <br/>
                        <br/>

                        <button onclick="return go_to_form(1)" class="btn btn-primary inc-button-prev">Back</button>

                        <button type="submit" id="resume-continue-btn" class="btn btn-primary inc-button-next" disabled>Upload your
                            resume to continue
                        </button>
                    </div>

                    <div
                        class="form-container application-form "
                        id="form-submit">
                        <h3>4. Submit Your Application</h3>

                        <p>There is no perfect application; however, there is a thoughtful and cohesive one. Take another look before
                            submitting:</p>

                        <p>
                            <a href="#" onclick="return go_to_form(0);">Basic Information</a><br/>
                            <a href="#" onclick="return go_to_form(1);">Questionnaire</a><br/>
                        </p>

                        <p>Once submitted, you will be contacted of your status within 2 business days.</p>

                        <button onclick="return go_to_form(2)" class="btn btn-primary inc-button-prev">Back</button>

                        <div id="application-submit-status"></div>
                        <form id="form-submit-application" action="/myhome/submit_application" method="post" role="form">
                            <button type="submit" class="btn btn-success inc-button-next">Submit Application</button>
                        </form>
                    </div>

                </div>

                <script>
                    //var form_ids = ["form-basic", "form-questions", "form-resume", "form-submit"];
                    var form_ids = ["form-basic", "form-questions", "form-submit"];
                    var current_form_index = 0;
                    var current_form = $('#' + form_ids[current_form_index]);
                    var increment_bar_list = $('#increment-bar-list');
                    var flavor_text = [
                        "Welcome Eric test! Let's complete your profile information.",
                        "Let's get to know you a little bit better!",
                        "Let's get to know you a little bit better!",
                        "Ready...Fire!"
                    ];

                    var flavor_text_container = $('#flavor-text-container');

                    $().ready(function () {
                        var resume_status_container = $('#resume-status');
                        var cover_status_container = $('#cover-status');
                        $('#resume_upload').fileupload({
                            dataType: 'json',
                            add: function (e, data) {
                                resume_status_container.html('Uploading...');
                                data.submit();
                            },
                            done: function (e, data) {
                                if (data.result.result == 1) {
                                    resume_status_container.removeClass('file-error');
                                    resume_status_container.html('Upload Completed - ' + data.result.name);
                                    $('#resume-continue-btn').html('Continue');
                                    $('#resume-continue-btn').prop('disabled', false);
                                }
                                else {
                                    resume_status_container.removeClass('file-error').addClass('file-error');
                                    resume_status_container.html('Upload Failed - ' + data.result.resume[0].error);
                                }
                            }
                        });

                        $('#cover_upload').fileupload({
                            dataType: 'json',
                            add: function (e, data) {
                                cover_status_container.html('Uploading...');
                                data.submit();
                            },
                            done: function (e, data) {
                                if (data.result.result == 1) {
                                    cover_status_container.removeClass('file-error');
                                    cover_status_container.html('Upload Completed - ' + data.result.name);
                                }
                                else {
                                    cover_status_container.removeClass('file-error').addClass('file-error');
                                    cover_status_container.html('Upload Failed - ' + data.result.cover[0].error);
                                }
                            }
                        });

                        $('#resume-continue-btn').click(function () {
                            show_next_form();
                        });

                        $('#form-submit-application').submit(function (event) {
                            var form = this;
                            $.get('/ajax/user/application_ready_check', function (data) {
                                var result = parseInt(data);
                                if (result == 1) {
                                    form.submit();
                                }
                                else {
                                    var container = $('div#application-submit-status');
                                    container.html('Application is not ready to be submitted. Check that all required forms are completed.');
                                }
                            });

                            return false;
                        });

                        jQuery.validator.addMethod("dob", function (value, element) {
                            var dobRegExp = /^([0-9]{2})\/([0-9]{2})\/([0-9]{4})$/;
                            var currentYear = new Date().getFullYear();
                            var currentMonth = new Date().getMonth();
                            var currentDay = new Date().getDate();

                            var match = dobRegExp.exec(value);
                            if (match == null || match.length == 0) return false;
                            var dobMonth = match[1];
                            var dobDay = match[2];
                            var dobYear = match[3];

                            var age = currentYear - dobYear;
                            var age_month = currentMonth - dobMonth;
                            var age_day = currentDay - dobDay;

                            if (age_month < 0 || (age_month == 0 && age_day < 0)) {
                                age = parseInt(age) - 1;
                            }

                            return (age >= 18);
                        }, "You must be at least 18 years of age.");

                        $('#form-questions form').validate({
                            submitHandler: function (form) {
                                var data = $(form).serialize();
                                $.post('/ajax/myhome/questionnaire', data, function (response) {
                                    if (response == 1) show_next_form();
                                });
                                return false;
                            }
                        });

                        $('#form-basic form').validate({
                            submitHandler: function (form) {
                                var data = $(form).serialize();
                                $.post('/ajax/myhome/basic', data, function (response) {
                                    if (response == 1) show_next_form();
                                });
                                return false;
                            },
                            rules: {
                                dob: {
                                    required: true,
                                    date: true,
                                    dob: true
                                },
                                phone: {
                                    required: true,
                                    phoneUS: true
                                }
                            }
                        });

                        $('#increment-bar-list li').click(function () {
                            if ($(this).hasClass('inc-active') || $(this).hasClass('inc-completed')) {
                                var index = $(this).index();
                                go_to_form(index);
                            }
                        });

                        update_flavor_text(current_form_index);
                    });

                    function update_flavor_text(index)
                    {
                        flavor_text_container.html(flavor_text[index]);
                    }

                    function go_to_form(index) {
                        if (index == current_form_index) return false;
                        update_flavor_text(index);
                        if (index < form_ids.length) {
                            var next_form = $('#' + form_ids[index]);

                            current_form_index = index;
                            current_form.animate(
                                {
                                    left: -1000
                                },
                                100,
                                function () {
                                    $(this).removeClass('.application-form--on').addClass('.application-form--off');
                                    $(this).css('left', 1000);
                                }
                            );

                            next_form.animate(
                                {
                                    left: 0
                                },
                                100,
                                function () {
                                    current_form = next_form;
                                    $(this).removeClass('.application-form--off').addClass('.application-form--on');
                                }
                            );

                            if (index > 0) {
                                var li = increment_bar_list.find('li').get(index - 1);
                                if ($(li).hasClass('inc-active')) $(li).removeClass('inc-active').addClass('inc-completed');

                                var li2 = increment_bar_list.find('li').get(index);
                                if ($(li2).hasClass('inc-inactive')) $(li2).removeClass('inc-inactive').addClass('inc-active');
                            }
                        }

                        return false;
                    }

                    function show_next_form() {
                        go_to_form(current_form_index + 1);
                    }

                    function update_increment_bar(index) {
                        if (index != undefined) {
                            var li = increment_bar_list.find('li').get(index);
                            $(li).removeClass('inc-active').addClass('inc-completed');
                        }
                        else {
                            var set_next_active = false;
                            increment_bar_list.find('li').each(function () {
                                if (set_next_active) {
                                    $(this).addClass('inc-active');
                                    set_next_active = false;
                                }
                                else if ($(this).hasClass('inc-active')) {
                                    $(this).removeClass('inc-active');
                                    $(this).addClass('inc-completed');
                                    set_next_active = true;
                                }
                            });
                        }
                    }
                </script>            </div>
        </div>
    </div>
</div>

<div id="question-container" style="display: none;">
    <form role="form" method="post" action="" id="question-form">
        <div id="question-response" class="alert alert-success"></div>
        <div class="form-group has-feedback">
            <input type="text" name="name" class="form-control" id="name" placeholder="Name" required />
        </div>
        <div class="form-group has-feedback">

            <input name="email" type="email" class="form-control" id="email" placeholder="Enter email" required/>
        </div>
        <div class="form-group has-feedback">
            <textarea name="question" class="form-control" required placeholder="Enter your question here"></textarea>
        </div>

        <button type="submit" class="btn btn-primary">Send Question</button>
    </form>
</div>


<script>
    $().ready(function(){
        $('#btn-question').fancybox({
            fitToView	: false,
            width		: '70%',
            height		: '70%',
            autoSize	: false,
            closeClick	: false,
            openEffect	: 'none',
            closeEffect	: 'none'
        });

        $('#question-form').submit(function(){
            var data = $(this).serialize();
            $.post('/ajax/misc/send_question', data, function(response){
                $('#question-response').show().html('Question has been successfully sent.');
            });

            return false;
        });
    });
</script>

</body>
</html>


