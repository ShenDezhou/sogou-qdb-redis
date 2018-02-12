#!/usr/bin/env perl
# -*- perl; coding: utf-8 -*-

use 5.010;
use Test::More;
use Tester;

my $tester = Tester->new;
$tester->start();

my $hash_name = "test-hash";

for ( 1..20 ) {
    $tester->try(
        "hset", $hash_name, "key-$_", "val-$_"
    );
}

ok($tester->try("hlen", $hash_name) == 20, 'command hset/hlen');
ok($tester->try("hget", $hash_name, "key-7") eq "val-7", 'command hget');

is($tester->try("hsetnx", $hash_name, "key-7", "val-7"), 0, 'command hsetnx 1');
is($tester->try("hsetnx", $hash_name, "key-21", "val-21"), 1, 'command hsetnx 2');

is($tester->try("hdel", $hash_name, "key-21"), 1, 'command hdel 1');
ok($tester->try("hlen", $hash_name) == 20, 'command hdel 2');

ok($tester->try("hexists", $hash_name, "key-7"), 'command hexists');

my %items_in_db = $tester->try("hgetall", $hash_name);
my %items_expected = map {; "key-$_" => "val-$_" } 1..20;
is_deeply(\%items_in_db, \%items_expected, "command hgetall");

my %keys_in_db = $tester->try("hkeys", $hash_name);
my %keys_expected = map {; "key-$_" } 1..20;
is_deeply(\%keys_in_db, \%keys_expected, "command hkeys");

my %vals_in_db = $tester->try("hvals", $hash_name);
my %vals_expected = map {; "val-$_" } 1..20;
is_deeply(\%vals_in_db, \%vals_expected, "command hvals");

$tester->stop;
done_testing;
