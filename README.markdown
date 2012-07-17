
================================================================================

                          Sigmah website GitHub project
  
                                     README

================================================================================

Presentation
------------

Sigmah website GitHub project is a project following the evolution of the code
of the Sigmah Drupal website.

Sigmah is Humanitarian Project Management Open Source Software.

You can see the production version of this website at: http://www.sigmah.org/


How to start ?
--------------
1. Install locally a PHP/MySQL server 
   i) If you are on Windows , you can use : [WAMP server](http://www.wampserver.com/en/)
   ii) If you are on MAC OSX, you can use : [XAMPP-MaxOSx Server](http://www.apachefriends.org/en/xampp-macosx.html)
   iii) If you are on Linux, you will have to setup a LAMP server, based on your distro:
   	a) Ubuntu and other Debian distributions:

	   	  ```
	   	    sudo apt-get install tasksel

		    sudo tasksel install lamp-server
		  ```
			(Note : tasksel does work on a few other Debian distributions, but it is mostly a ubuntu specific package installer)
			
		 If tasksel is not available for you distribution, you can try the following set of commands which use apt-get
		   ```
			apt-get install update
			apt-get install apache2 php5 libapache2-mod-php5
			apt-get install mysql-server mysql-client php5-mysql
		   ```
         b) Other linux distributions which support yum :
	    	  ```
			yum update
			yum install httpd
			yum install php php-pear php-mysql
			yum install mysql-server mysql
		  ```
	
	(If still the apache server doesnot start or process php files, you can try restarting the services once)
	    	  ```
			sudo /etc/init.d/apache2 restart

			sudo /etc/init.d/mysqld restart
		  ``` 

		      or

		   ```
			sudo service httpd restart

			sudo service mysqld restart
		   ```
			Whichever is applicable for your distribution
		 
2. Create a database "sigmah_website" accessible from the user 'root' in your
  MySQL database and import the database from the sql file provided.
		
		You can either achieve this through the phpMyadmin provided by default in you WAMP or XAMPP stacks, or if you are on Linux and you dont have phpMyadmin , you can use the following command :
		  ```
			mysql -u root -p -e "CREATE DATABASE sigmah_website;"
		  ```
		  Then it will ask for password, the default password is blank, so a simple enter should do the job if it a newly set up LAMP stack, or you will have to enter thr root password setup on your mysql server.

		  Now you have created the database, you will have to import the database dump present. Find the file named sigmah_website.sql file. And if you are on Windows or MAC OSx, use the phpMyadmin interface to import the database contents into your newly created database, or if you are on linux, then use the following command
		  
		  ```
		  mysql -u root -p -d sigmah_website < <path_to_sql_file>
		  ```

			This will again ask for password, either give the default blank password or give your custom password.
		  	  
3. Create an Apache alias named "sigmah-website" to your local copy of the 
  content of this Git repo.

4. Configure you apache server to let the sigmah-website use clean urls.

   Cleans URLS are enabled by default, so for you to be able to use it, your apache server has to allow the sigmah-website project override the default settings bu custom .htaccess files, so locate the operative httpd.conf file 
or apache2.conf file whichever is applicable, and add the following lines

   		```
		<Directory /var/www/sigmah-website>
			   AllowOverride All
		</Directory>
		```
		(Assuming /var/www/sigmah-website is the path to your project directory )

5. Open in your web browser the URL pointing to this alias 
  (like http://127.0.0.1:8000/sigmah-website/, but it depends on your local
  Apache server config).
6. Log on to the website with the login/password "user3"/"sigmah" to log on with
  a user account which has all privileges.
=> You're in !

7. Now for the integration of the sigmah website with the issue tracker, the database config details are to be updated at :

```
sites/all/modules/sigmah/sigmah.module
```

in the function defination of connectToMantisDatabase(), you will find an associtive array :
// Database config details
$db = array(
'server' => '127.0.01',
'username' => 'root',
'password' => '',
'database' => 'mantis'
);

there you can replace the username,password,database name,server address, etc of the Mantis issue tracker, and you are ready to go :)



For information: all user accounts have "sigmah" as password. 

If you have any queries or troubles setting up the project, drop a
mail to sigmah-dev@googlegroups.com , we will be more than happy to
help you get code up and running :).

8. For Integration of the mantis issue tracker with the github
repository :
  1. Open the Mantis Issue Tracker Administrator account
  2. Go to Manage > Manage Plugins
  3. Two Plugins named "GitHub Integration 0.16" and "Source Control
  Integration 0.16.4" would be present, which will have to be
  installed
  4. Now you will have a new "Repositories" link in the top menu
  5. Inside "Repositories", Type the name of the Repository in the
  Name field, and "Github" in the type field.
  6. Then fill the Github Username(osarrat) and the Github project
  URL(https://github.com/osarrat/sigmah-website) and the github
  repository name (sigmah-website) and
  update the repository
  7. Then Click on "Import Everything" to fetch the commit details
  from the github servers
  
  8. Then look for the "Configuration" button inside the
  "Repositories" link, and use the following values for the form :
  
    *View Threshold : viewer
    *Update Threshold : updater
    *Manage Threshold : administrator
    *Set Username threshold : developer
    *Main Menu Links : Repositories (Checked) , Search(Depends upon your
  need)
    *Enabled Features : Repository Statistics (Checked),ChangesetLinking (Checked),Branch Mappings (Uncheked),Resolve Fixed Issues (Checked),Bug Fixed Message(Checked),Porting Status (Checked or Unchecked depending upon ur need)
	* Bug Link Regex Pass 1 :
  /(?:bugs?|issues?|reports?)+\s+(?:#(?:\d+)[,\.\s]*)+/i 
  (the regex can be changed ofcourse if you want any other pattern)
    * Bug Link Regex Pass 2 : /#?(\d+)/
	* Bug Fixed Regex Pass 1 :
      /(?:fixe?d?s?|resolved?s?)+\s+(?:#(?:\d+)[,\.\s]*)+/i
	* Bug Fixed Regex Pass 2	 :   /#?(\d+)/
	
	* Bug Fixed Status : Resolved
	* Bug Fixed Resolution : fixed
	* Bug Fixed Message Template : Fix committed to $1 branch of $5
	* Bug Fixed Assign To Committer	 : Checked 
    * API Key :
      ANY-SECRET-ALPHANUMERIC-KEY-YOU-WISH-WILL-HAVE-TO-USE-THE-SAME-ON-SIGMAH-GITHUB-REPO
	* Allow Remote Check-in : Unchecked
	* Allowed Addresses : 
	  * 207.97.227.253
 	  * 50.57.128.197
	  * 108.171.174.178
	  
	* Allow Remote Imports : Unchecked
	
	* Allowed Addresses : 
		 * 207.97.227.253
 	     * 50.57.128.197
	     * 108.171.174.178
		 
   9) And then finally update configuration :D !! :)
   
   
   10) Now, On this Github projects Page , Go to Admin > Service Hooks
   > Mantis Bt 
   11) For URL use the base URL of the project (in the case of the
   live website : http:www.sigmah.org/issues
   12) And for API key, use the same API key used on the Mantis
   Configuration Page
   13) now test Hook, to check if the payload is being successfully
   deployed !!
   14) Update Settings, and you are ready to go :) !! #phew !
