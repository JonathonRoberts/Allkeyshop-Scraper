#!/usr/bin/perl
use strict;
use warnings;
use WWW::Mechanize;
use HTML::TreeBuilder::XPath;
use Email::Sender::Simple qw(sendmail);
use Email::Sender::Transport::SMTP ();
use Email::Simple ();
use Email::Simple::Creator ();

my $message;
sub scrape{

	my $url = $_[0];
	my $thresh = $_[1];

	my $mech=WWW::Mechanize->new();
	$mech->get($url);
	my $tree=HTML::TreeBuilder::XPath->new();
	$tree->parse($mech->content);

	my $content;# XPath output
	# my @nodes = $tree->findnodes('id(\'euros\')/div[2]/p[2]/span[3]');
	my @nodes = $tree->findnodes('id(\'euros\')/div/table/tr[2]/td/del/span/div/span');
	foreach(@nodes){
		$content = $_->findvalue('@content');
		if($content>$thresh){
			$message .= "$url is on offer for $content\n";
		}
	}
}

my $config = "~/.allkeyshopconfig";
my $i = 0;
my $url;
my $price;
my $thresh;

open(my $fh, "<", $config)
	or die "Can't open < $config: $!";
while(<$fh>){
	if($i++%2){
		chomp;
		$thresh = $_;
		&scrape($url, $thresh);
	}
	else{
		chomp;
		$url = $_;
	}
}
close $fh;

if(defined($message)){
	print $message;
	my $smtpserver = 'smtp.server.com';
	my $smtpport = 587;
	my $smtpuser   = 'email@server.com';
	my $smtppassword = 'password123';

	my $transport = Email::Sender::Transport::SMTP->new({
	host => $smtpserver,
	port => $smtpport,
	ssl  => 'starttls',
	port => $smtpport,
	sasl_username => $smtpuser,
	sasl_password => $smtppassword,

	});

	my $email = Email::Simple->create(
	header => [
	To      => 'recipient@server.com',
	From    => $smtpuser,
	Subject => 'Allkeyshop Offers',
	],
	body => $message,
	);

	sendmail($email, { transport => $transport });
	exit 0;
}
	exit 0;

