package com.environmentaldata.environmentaldata;
 
import org.springframework.data.jpa.repository.JpaRepository;
 
public interface UserRepository extends JpaRepository<users, Long> {
 
}