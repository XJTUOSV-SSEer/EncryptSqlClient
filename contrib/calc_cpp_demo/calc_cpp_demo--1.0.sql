CREATE OR REPLACE FUNCTION test_1()
RETURNS integer
AS '$libdir/calc_cpp_demo'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION add_ex(BYTEA,BYTEA)
RETURNS BYTEA
AS '$libdir/calc_cpp_demo'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION test_enc(text,int)
RETURNS text
AS '$libdir/calc_cpp_demo'
LANGUAGE C STRICT;

CREATE AGGREGATE SUM_ex(bytea)
(
        STYPE = bytea,
        SFUNC = add_ex
);

CREATE OR REPLACE FUNCTION sum_by_row(k text)
RETURNS bytea AS
$$
DECLARE
tmp_res bytea := '123456';
       ekey text;
	   res bytea;
	   counter INT := 0;
BEGIN
drop table if EXISTS tmp_1;
create temp table tmp_1(v1 bytea);
    loop
        ekey := test_enc(k,counter);
		if NOT EXISTS(SELECT enc_value from kvtest where enc_key = ekey) then exit;
end if;
SELECT enc_value into tmp_res from kvtest where enc_key = ekey;
insert into tmp_1 VALUES (tmp_res);
counter := counter + 1;
end loop;
select SUM_ex(v1) into res FROM tmp_1;
return res;
END
$$
LANGUAGE plpgsql;
